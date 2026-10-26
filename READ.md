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
- ESP-IDF para compilar y cargar el firmware.
- Python 3 para ejecutar el software de la Raspberry Pi.
- Dependencias utilizadas por el servidor y la interfaz gráfica.


## Resultados

El sistema permite adquirir y visualizar mediciones del acelerómetro y giroscopio. El informe presenta el análisis de las pruebas realizadas y una comparación de las estadísticas de transmisión mediante TCP y UDP.
