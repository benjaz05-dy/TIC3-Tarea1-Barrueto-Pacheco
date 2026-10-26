Este documento contiene los prompts utilizados por el equipo durante el desarrollo del proyecto, usamos ChatGPT y Gemini como herramientas.

Etapa: Conexión ESP32-S3 con Raspberry Pi.

1\. Tenemos este codigo base del driver del BMI270 en un solo archivo (main.c) para ESP32-S3 con ESP-IDF que lee acelerometro y giroscopio por I2C pero tiene la frecuencia y el rango fijos en el codigo. Necesitamos conectarlo por WiFi como cliente TCP hacia la Raspberry Pi  para que reciba el comando START, frecuencia, rango, cantidad, modo desde la Raspberry, cambie los registros  del BMI270 antes de cada sesión y ajuste la fórmula de conversión  según el rango elegido

Resultado obtenido: Se obtuvo una propuesta para conectar el ESP32 mediante Wi-Fi y TCP con la Raspberry Pi, permitiendo configurar la frecuencia, el rango y la cantidad de muestras del BMI270 desde la interfaz gráfica.

Uso de la respuesta: Se utilizó como base para implementar la comunicación entre el ESP32 y la Raspberry Pi.

Etapa: Implementación de modos secuencial y paralelo.

2\. Ahora necesitamos que el ESP32 soporte los dos modos que pide la tarea: SECUENCIAL (donde lee el sensor por I2C y envía por socket de forma bloqueante en un mismo ciclo) y PARALELO  y los dos núcleos del ESP32-S3 para que la lectura I2C y el envío por WiFi no se bloqueen mutuamente. Además, separa la parte de WiFi y sockets en tcp\_cliente.c y tcp\_cliente.h.

Resultado obtenido: Se propuso implementar ambos modos de adquisición utilizando FreeRTOS para separar la lectura del sensor y la transmisión de datos. También se propuso organizar la comunicación TCP en archivos independientes.

Uso de la respuesta: Se tomó como referencia para desarrollar los modos secuencial y paralelo, realizando ajustes al código.

Etapa: Implementación de protocolos TCP y UDP.

3\. Aquí tienes nuestros archivos actuales. Actualmente todo funciona por TCP, pero la tarea pide que desde la GUI de la Raspberry Pi se pueda cambiar entre protocolo TCP y UDP en cualquier momento (con TCP por defecto al iniciar) sin reescribir código, y poder comparar el tráfico TCP vs UDP. Modifica los archivos para agregar la selección de TCP/UDP en la GUI, el envío por UDP desde el ESP32 y una tabla comparativa en la interfaz.

Resultado obtenido: Se incorporó la posibilidad de seleccionar TCP o UDP desde la interfaz gráfica, además de una tabla para comparar las estadísticas de transmisión de ambos protocolos.

Uso de la respuesta: Se utilizó el código generado con modificaciones para integrar TCP y UDP al sistema.

Etapa: Mejoras de la interfaz gráfica.

4\. Oye necesito que me ayudes a terminar esta tarea, estas son las cosas que debemos agregar por es lo que me falta, asi que necesito que hagas esos cambios.

Esto hay que integrar:

Agregar una opcion en la interfaz para ver la grafica sea continua (hasta el inf)  y que sea con tiempo, ademas de las opciones que ya tenemos.

agregar forma UDP y comparar con TCP (es que ya tenemos).

opción de elegir en la interfaz para ver en la grafica la aceleración o giroscopio.

cambiar la interfaz y que se vea mas moderno

Resultado obtenido: Se agregaron opciones de visualización continua y por tiempo, selección entre acelerómetro y giroscopio, y mejoras en el diseño visual de la interfaz.

Uso de la respuesta: Se utilizaron las modificaciones propuestas para mejorar la interfaz gráfica y agregar nuevas funciones.

Etapa: Comprensión del funcionamiento del sistema.

5\. okey ahora quiero que me ayudes a entender lo que yo vere en la interfaz y que es o como se diferencia.

lo de paralelo o secuencial, las muestras, TCP o UDP, la lectura I2C, sevidor sockets y dime tambien sobre la tabla de comparacion, que es lo que compara y como saca esos datos

Resultado obtenido: Se obtuvo una explicación de los modos secuencial y paralelo, las muestras, la comunicación I2C, los sockets y los protocolos TCP y UDP. Esto permitió comprender mejor las funciones de la interfaz y sus estadísticas.

Uso de la respuesta: Se utilizó para comprender el funcionamiento del sistema. No se generó código.

Etapa: Corrección de errores en Raspberry Pi.

6\. Al ejecutar el script en la Raspberry Pi me salió un error en la terminal,   tkinter.TclError: bad stickiness value "center":

Resultado obtenido: Se identificó un error relacionado con el posicionamiento de elementos en Tkinter y se propuso corregir el parámetro que impedía ejecutar correctamente la interfaz.

Uso de la respuesta: Se aplicó la corrección propuesta para solucionar el error de Tkinter.

Etapa: Corrección de adquisición continua y por tiempo.

7\. Probé la interfaz nueva y en modo por muestras funciona bien de momento, pero cuando pongo modo continuo (Inf) o por tiempo se detiene todo con 0 muestras guardadas, o cuando pongo por ejemplo 10 segundos el contador de tiempo salta directo de 0 a 10 segundos en la primera muestra y se corta. Revisa por qué pasa eso y arréglalo en el código de la Raspberry.

Resultado obtenido: Se revisó el control del tiempo y el procesamiento de las muestras, realizando ajustes para evitar que la adquisición terminara antes de lo esperado.

Uso de la respuesta: Se realizaron ajustes al código para corregir los problemas de adquisición continua y por tiempo.

Etapa: Comunicación I2C con el BMI270.

8\. Explícame cómo funciona la comunicación I2C entre el ESP32 y el sensor BMI270.

Resultado obtenido: Se obtuvo una explicación del funcionamiento del bus I2C y de cómo el ESP32 se comunica con el BMI270 para configurar sus registros y leer las mediciones.

Uso de la respuesta: Se utilizó como explicación para comprender la comunicación I2C. No se generó código.

Etapa: Configuración del acelerómetro.

9\. Cual es la diferencia entren los rangos de 2g, 4g y 8g en el acelerómetro

Resultado obtenido: Se explicó la diferencia entre los rangos de ±2g, ±4g y ±8g, comprendiendo cómo afectan la sensibilidad y el límite de aceleración que puede medir el sensor.

Uso de la respuesta: Se utilizó para comprender los rangos del acelerómetro. No se generó código.

Etapa: Estudio de protocolos de comunicación.

10\. Explícame cual es la diferencia entre TCP y UDP

Resultado obtenido: Se explicaron las diferencias entre TCP y UDP, principalmente los mecanismos de entrega, confirmación y retransmisión de datos. Esta información permitió comprender mejor la comparación de protocolos del proyecto.

Uso de la respuesta: Se utilizó como apoyo para comprender las diferencias entre TCP y UDP. No se generó código.

Etapa: Elaboración del informe técnico.

11\. Oye debemos hacer el informe. Entonces necesito que tú me mandes un zip en LaTeX, el informe debe de ser de 5 a 6 hojas y que tenga la info que tenemos aca

Resultado obtenido: Se solicitó la elaboración del informe en LaTeX, considerando la arquitectura del sistema, el funcionamiento del sensor, los modos de adquisición y la comparación entre TCP y UDP.

Uso de la respuesta: Se utilizó como base para elaborar el informe técnico, realizando modificaciones en su contenido.

Etapa: Revisión de conclusiones del informe.

12\. Ayúdame a mejorar la redacción de la conclusión del informe.

Resultado obtenido: Se mejoró la redacción de la conclusión, corrigiendo afirmaciones sobre TCP y UDP para que fueran coherentes con los resultados obtenidos durante las pruebas.

Uso de la respuesta: Se utilizaron las correcciones propuestas para mejorar la conclusión del informe.

Etapa: Análisis de resultados TCP y UDP.

13\. Revisa si la explicación de los resultados TCP y UDP coincide con los datos de nuestras pruebas.

Resultado obtenido: Se revisaron las estadísticas de transmisión, incluyendo pérdida de muestras, frecuencia efectiva y jitter. Se corrigieron algunas interpretaciones para que el análisis coincidiera con los valores mostrados por la interfaz.

Uso de la respuesta: Se utilizaron las observaciones para corregir la interpretación de los resultados TCP y UDP.

Etapa: Documentación del GitHub.

14\. Oye con lo que tengo en la imagen puedes hacer un readme para el inicio

Resultado obtenido: Se elaboró una propuesta de README principal con la descripción del proyecto, estructura de carpetas, requisitos e instrucciones generales de instalación y ejecución.

Uso de la respuesta: Se utilizó la propuesta para elaborar y organizar el README principal.



