#ifndef TCP_CLIENTE_H
#define TCP_CLIENTE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int frecuencia_hz;
    int rango_g;
    int cantidad;
    bool paralelo;
} tcp_config_t;

void wifi_iniciar(void);

bool tcp_conectar(void);

bool tcp_recibir_config(tcp_config_t *config);

bool tcp_enviar_imu(
    uint32_t secuencia,
    uint32_t tiempo_ms,
    float ax, float ay, float az,
    float gx, float gy, float gz,
    int odr_hz,
    int rango_g,
    int periodo_ms
);

bool tcp_enviar_linea(const char *mensaje);

void tcp_desconectar(void);

#endif