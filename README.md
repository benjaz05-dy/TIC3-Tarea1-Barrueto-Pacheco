# TIC3 - Tarea 1: ESP32-S3, BMI270 y Raspberry Pi

De Benjamín Barrueto y Benjamín Pacheco.

## Descripción del proyecto

Este proyecto implementa un sistema de adquisición y transmisión de datos de un sensor inercial BMI270 mediante un microcontrolador ESP32-S3 y una Raspberry Pi.

El ESP32-S3 se encarga de adquirir las mediciones del acelerómetro y giroscopio, mientras que la Raspberry Pi ejecuta el servidor y una interfaz gráfica para visualizar y registrar los datos.

El sistema contempla modos de adquisición secuencial y paralelo, además de pruebas de comunicación mediante TCP y UDP.

## Estructura del repositorio

- esp32: código fuente del microcontrolador ESP32-S3.
- raspberry: código del servidor y de la interfaz gráfica.
- videos: demostraciones del funcionamiento del sistema.
- informe: documentación y resultados del proyecto.
- prompts.md: registro de los prompts utilizados durante el desarrollo.

## Requisitos

**Hardware:**
- ESP32-S3.
- Sensor BMI270.
- Raspberry Pi.
- Conexión I2C entre ESP32-S3 y BMI270.
- Conexión de red entre ESP32-S3 y Raspberry Pi.

**Software:**
- ESP-IDF para compilar y cargar el firmware del ESP32-S3.
- Python 3 para ejecutar el servidor y la interfaz gráfica.
- Biblioteca Matplotlib para visualizar las mediciones.
- Tkinter para la interfaz gráfica.
- Bibliotecas estándar de Python para comunicación por sockets y almacenamiento CSV.

## Instalación y ejecución

### ESP32-S3

1. Instalar y configurar el entorno ESP-IDF.
2. Abrir el proyecto del firmware ESP32-S3.
3. Configurar los pines I2C utilizados para comunicarse con el BMI270.
4. Configurar los parámetros Wi-Fi y la dirección IP del servidor Raspberry Pi.
5. Compilar y cargar el firmware en el ESP32-S3.

### Raspberry Pi

1. Tener instalado Python 3.
2. Instalar las dependencias necesarias, principalmente Matplotlib y Tkinter.
3. Abrir el archivo codigoraspberry.py ubicado en la carpeta raspberry.
4. Ejecutar el programa desde un entorno gráfico de la Raspberry Pi.
5. Esperar a que el ESP32-S3 establezca la conexión con el servidor.

El servidor utiliza el puerto 5000 para las comunicaciones TCP y UDP.

## Funcionamiento del sistema

Desde la interfaz gráfica de la Raspberry Pi se pueden seleccionar diferentes parámetros de adquisición:

- Frecuencia de muestreo: 25, 50 o 100 Hz.
- Rango del acelerómetro: ±2g, ±4g, ±8g o ±16g.
- Modo de adquisición: secuencial o paralelo.
- Protocolo de comunicación: TCP o UDP.
- Duración de adquisición: por cantidad de muestras, por tiempo o de forma continua.

La interfaz permite visualizar las mediciones del acelerómetro y giroscopio en tiempo real, además de comparar estadísticas de comunicación de los protocolos TCP y UDP.

## Almacenamiento de datos

El programa de la Raspberry Pi genera automáticamente archivos CSV para registrar las muestras recibidas.

Los archivos contienen información de aceleración, velocidad angular, tiempo, protocolo utilizado y parámetros de configuración de la adquisición.


## Resultados

El sistema permite adquirir, transmitir, visualizar y almacenar mediciones del acelerómetro y giroscopio del BMI270.

El informe técnico presenta la arquitectura del sistema, las decisiones de diseño y una comparación de las estadísticas de transmisión TCP y UDP obtenidas durante las pruebas.

## Demostración y documentación

Los videos del funcionamiento del sistema se encuentran en la carpeta videos.

El informe técnico se encuentra en el archivo Informe_TIC3_Tarea1.pdf.

El registro de consultas realizadas a herramientas de inteligencia artificial durante el desarrollo del proyecto se encuentra en prompts.md.



