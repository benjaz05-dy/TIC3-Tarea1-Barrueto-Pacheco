Estas grabaciones muestran el funcionamiento de un sistema para adquirir y transmitir mediciones de movimiento. El sensor inercial BMI270 registra aceleración y velocidad angular. Un microcontrolador ESP32-S3 obtiene las mediciones y las envía a una Raspberry Pi 5, que recibe los datos para mostrarlos y almacenarlos desde una interfaz gráfica.

La comunicación utiliza una arquitectura cliente-servidor: la Raspberry Pi ejecuta el programa servidor y espera una conexión entrante; el ESP32-S3 se conecta a la dirección IP y al puerto configurados. Para que la conexión funcione, ambos dispositivos deben estar conectados a la misma red Wi-Fi. Una vez conectados, la interfaz permite configurar y comenzar una sesión de adquisición.

## Primera grabación: adquisición continua

En el [primer video] se demuestra la adquisición continua mediante TCP. La frecuencia configurada es de 50 Hz, es decir, se espera obtener una muestra cada 20 milisegundos. El acelerómetro utiliza un rango de ±4g y las lecturas se procesan en modo secuencial: el microcontrolador lee el sensor, procesa la medición y luego transmite los datos, una operación tras otra.

La duración continua no establece de antemano una cantidad límite de muestras. La sesión permanece activa hasta que se detiene, lo que permite observar el flujo de mediciones durante el funcionamiento del sistema. TCP mantiene una conexión y entrega los datos en orden, por lo que resulta útil cuando se prioriza la recepción íntegra de la información.

## Segunda grabación: adquisición por cantidad de muestras

En el [segundo video] se configura una sesión de 1000 muestras a 100 Hz, con un rango del acelerómetro de ±8g. El período nominal es de 10 milisegundos entre muestras y, si la frecuencia se mantiene constante, la captura dura aproximadamente 10 segundos.

Esta demostración utiliza lectura paralela. Las tareas de adquisición y transmisión se ejecutan por separado, y una cola de FreeRTOS permite guardar temporalmente las muestras mientras se procesan para su envío. Así, la lectura del sensor depende menos de los tiempos que toma la comunicación. El protocolo seleccionado es UDP, que envía datagramas sin confirmación ni retransmisión; por eso reduce esperas, aunque no garantiza que todos los datos lleguen.

## Diferencias entre las pruebas

Las grabaciones muestran dos formas de configurar el sistema. La primera prioriza una captura continua, con procesamiento sencillo y entrega ordenada mediante TCP. La segunda aumenta la frecuencia de muestreo y separa la lectura del envío para registrar una cantidad definida de mediciones. El rango de ±8g permite medir aceleraciones mayores que ±4g, aunque ofrece menor resolución nominal. UDP reduce la espera asociada a la transmisión, pero puede perder datagramas.
