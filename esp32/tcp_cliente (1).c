#include "tcp_cliente.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "nvs_flash.h"

#define WIFI_SSID "iPhone de Eustas"
#define WIFI_PASS "Pachec1652"

// IP DE LA RASPBERRY
#define SERVER_IP "172.20.10.10"
#define SERVER_PORT 5000

#define WIFI_OK_BIT BIT0

static const char *TAG_TCP = "TCP_BMI270";
static EventGroupHandle_t wifi_eventos;
static int socket_tcp = -1;
static int socket_udp = -1;
static struct sockaddr_in destino_udp = {0};
static bool modo_udp_activo = false;

// Gestion de eventos WiFi
static void evento_wifi(
    void *arg,
    esp_event_base_t base,
    int32_t id,
    void *data
) {
    if (base == WIFI_EVENT &&
        id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }

    if (base == WIFI_EVENT &&
        id == WIFI_EVENT_STA_DISCONNECTED) {
        xEventGroupClearBits(wifi_eventos, WIFI_OK_BIT);
        esp_wifi_connect();
    }

    if (base == IP_EVENT &&
        id == IP_EVENT_STA_GOT_IP) {
        xEventGroupSetBits(wifi_eventos, WIFI_OK_BIT);
        ESP_LOGI(TAG_TCP, "WiFi conectado, IP obtenida");
    }
}

// Iniciar conexion WiFi al telefono
void wifi_iniciar(void) {

    esp_err_t err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
        err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    ESP_ERROR_CHECK(err);

    wifi_eventos = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID,
        &evento_wifi, NULL
    ));

    ESP_ERROR_CHECK(esp_event_handler_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP,
        &evento_wifi, NULL
    ));

    wifi_config_t wifi_config = {0};

    snprintf(
        (char *)wifi_config.sta.ssid,
        sizeof(wifi_config.sta.ssid),
        "%s", WIFI_SSID
    );

    snprintf(
        (char *)wifi_config.sta.password,
        sizeof(wifi_config.sta.password),
        "%s", WIFI_PASS
    );

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    ESP_ERROR_CHECK(esp_wifi_set_config(
        WIFI_IF_STA, &wifi_config
    ));

    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG_TCP, "Conectando al hotspot...");

    xEventGroupWaitBits(
        wifi_eventos,
        WIFI_OK_BIT,
        pdFALSE,
        pdTRUE,
        portMAX_DELAY
    );

    ESP_LOGI(TAG_TCP, "WiFi listo");
}

// Cerrar sockets anteriores
static void tcp_cerrar(void) {
    if (socket_tcp >= 0) {
        close(socket_tcp);
        socket_tcp = -1;
    }
    if (socket_udp >= 0) {
        close(socket_udp);
        socket_udp = -1;
    }
}

// Establecer conexion TCP y preparar socket UDP
bool tcp_conectar(void) {

    tcp_cerrar();

    EventBits_t bits = xEventGroupWaitBits(
        wifi_eventos,
        WIFI_OK_BIT,
        pdFALSE,
        pdTRUE,
        pdMS_TO_TICKS(5000)
    );

    if (!(bits & WIFI_OK_BIT)) {
        ESP_LOGW(TAG_TCP, "WiFi no disponible");
        return false;
    }

    socket_tcp = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (socket_tcp < 0) {
        ESP_LOGE(TAG_TCP, "No se pudo crear socket");
        return false;
    }

    struct sockaddr_in destino = {0};

    destino.sin_family = AF_INET;
    destino.sin_port = htons(SERVER_PORT);

    if (inet_pton(
        AF_INET,
        SERVER_IP,
        &destino.sin_addr
    ) != 1) {
        tcp_cerrar();
        return false;
    }

    ESP_LOGI(TAG_TCP, "Conectando a %s:%d",
             SERVER_IP, SERVER_PORT);

    if (connect(
        socket_tcp,
        (struct sockaddr *)&destino,
        sizeof(destino)
    ) != 0) {
        ESP_LOGW(TAG_TCP, "Conexion TCP fallida");
        tcp_cerrar();
        return false;
    }

    // Crear socket UDP para cuando se elija protocolo UDP
    socket_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (socket_udp >= 0) {
        destino_udp = destino;
    }

    ESP_LOGI(TAG_TCP, "Conexion TCP establecida (UDP preparado)");
    return true;
}

// Verificar sin bloquear si la Raspberry envio STOP
bool tcp_verificar_stop(void) {
    if (socket_tcp < 0) {
        return true;
    }

    char buf[32];
    ssize_t n = recv(socket_tcp, buf, sizeof(buf) - 1, MSG_DONTWAIT);

    if (n > 0) {
        buf[n] = '\0';
        if (strstr(buf, "STOP") != NULL) {
            ESP_LOGI(TAG_TCP, "Comando STOP recibido");
            return true;
        }
    } else if (n == 0) {
        return true;
    } else {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            return true;
        }
    }

    return false;
}

// Enviar datos de una muestra real (TCP o UDP)
bool tcp_enviar_imu(
    uint32_t secuencia,
    uint32_t tiempo_ms,
    float ax, float ay, float az,
    float gx, float gy, float gz,
    int odr_hz,
    int rango_g,
    int periodo_ms
) {

    if (socket_tcp < 0) {
        return false;
    }

    char paquete[384];

    int longitud = snprintf(
        paquete,
        sizeof(paquete),
        "{\"type\":\"imu\","
        "\"proto\":\"%s\","
        "\"seq\":%lu,"
        "\"t_ms\":%lu,"
        "\"acc_x_ms2\":%.5f,"
        "\"acc_y_ms2\":%.5f,"
        "\"acc_z_ms2\":%.5f,"
        "\"gyr_x_rads\":%.5f,"
        "\"gyr_y_rads\":%.5f,"
        "\"gyr_z_rads\":%.5f,"
        "\"odr_hz\":%d,"
        "\"rango_g\":%d,"
        "\"periodo_ms\":%d}\n",
        modo_udp_activo ? "UDP" : "TCP",
        (unsigned long)secuencia,
        (unsigned long)tiempo_ms,
        ax, ay, az,
        gx, gy, gz,
        odr_hz, rango_g, periodo_ms
    );

    if (longitud < 0 || longitud >= (int)sizeof(paquete)) {
        ESP_LOGE(TAG_TCP, "Error formando JSON");
        return false;
    }

    if (modo_udp_activo) {
        if (socket_udp < 0) {
            ESP_LOGW(TAG_TCP, "Socket UDP no disponible");
            return false;
        }

        int n = sendto(
            socket_udp,
            paquete,
            longitud,
            0,
            (struct sockaddr *)&destino_udp,
            sizeof(destino_udp)
        );

        if (n < 0) {
            ESP_LOGW(TAG_TCP, "Fallo de envio UDP");
            return false;
        }

        ESP_LOGI(TAG_TCP, "Muestra %lu enviada (UDP)",
                 (unsigned long)secuencia);
        return true;
    }

    // TCP puede enviar menos bytes de los pedidos
    int enviados = 0;

    while (enviados < longitud) {
        int n = send(
            socket_tcp,
            paquete + enviados,
            longitud - enviados,
            0
        );

        if (n <= 0) {
            ESP_LOGW(TAG_TCP, "Fallo de envio TCP");
            tcp_cerrar();
            return false;
        }

        enviados += n;
    }

    ESP_LOGI(TAG_TCP, "Muestra %lu enviada (TCP)",
             (unsigned long)secuencia);

    return true;
}

// Cerrar conexion actual
void tcp_desconectar(void) {
    tcp_cerrar();
}

// Enviar un comando o fin de sesion terminado en '\n'.
bool tcp_enviar_linea(const char *mensaje) {
    if (socket_tcp < 0 || mensaje == NULL) {
        return false;
    }

    size_t longitud = strlen(mensaje);
    size_t enviados = 0;

    while (enviados < longitud) {
        ssize_t n = send(socket_tcp, mensaje + enviados, longitud - enviados, 0);
        if (n <= 0) {
            ESP_LOGW(TAG_TCP, "Fallo al enviar mensaje TCP");
            tcp_cerrar();
            return false;
        }
        enviados += (size_t)n;
    }
    return true;
}

// Esperar configuracion recibida desde Raspberry por la misma conexion TCP.
bool tcp_recibir_config(tcp_config_t *config) {
    if (config == NULL || socket_tcp < 0) {
        return false;
    }

    char mensaje[100];

    while (1) {
        size_t posicion = 0;
        bool demasiado_largo = false;

        // TCP no conserva limites de mensajes: leer hasta '\n'.
        while (1) {
            char caracter;
            ssize_t n = recv(socket_tcp, &caracter, 1, 0);
            if (n <= 0) {
                ESP_LOGW(TAG_TCP, "Conexion cerrada o error de recepcion");
                tcp_cerrar();
                return false;
            }
            if (caracter == '\n') {
                break;
            }
            if (caracter == '\r') {
                continue;
            }
            if (posicion < sizeof(mensaje) - 1) {
                mensaje[posicion++] = caracter;
            } else {
                demasiado_largo = true;
            }
        }
        mensaje[posicion] = '\0';

        if (strcmp(mensaje, "STOP") == 0 || posicion == 0) {
            continue;
        }

        ESP_LOGI(TAG_TCP, "Comando recibido: %s", mensaje);

        int frecuencia = 0;
        int rango = 0;
        int cantidad = -1;
        char modo[16] = {0};
        char protocolo[16] = "TCP";

        int campos = sscanf(mensaje, "START %d %d %d %15s %15s",
                            &frecuencia, &rango, &cantidad, modo, protocolo);

        bool frecuencia_valida = frecuencia == 25 || frecuencia == 50 || frecuencia == 100;
        bool rango_valido = rango == 2 || rango == 4 || rango == 8 || rango == 16;
        bool modo_valido = strcmp(modo, "SECUENCIAL") == 0 ||
                           strcmp(modo, "PARALELO") == 0;
        bool proto_valido = strcmp(protocolo, "TCP") == 0 ||
                            strcmp(protocolo, "UDP") == 0;

        if (!demasiado_largo && (campos == 4 || campos == 5) && frecuencia_valida &&
            rango_valido && cantidad >= 0 && cantidad <= 1000000 && modo_valido && proto_valido) {
            config->frecuencia_hz = frecuencia;
            config->rango_g = rango;
            config->cantidad = cantidad;
            config->paralelo = strcmp(modo, "PARALELO") == 0;
            config->usar_udp = strcmp(protocolo, "UDP") == 0;
            modo_udp_activo = config->usar_udp;
            return true;
        }

        if (!tcp_enviar_linea("ERR,COMANDO_INVALIDO\n")) {
            return false;
        }
    }
}
