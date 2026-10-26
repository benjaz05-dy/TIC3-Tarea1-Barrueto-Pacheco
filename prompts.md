&#x20;1. Tenemos este codigo base del driver del BMI270 en un solo archivo (main.c) para ESP32-S3 con ESP-IDF que lee acelerometro y giroscopio por I2C pero tiene la frecuencia y el rango fijos en el codigo. Necesitamos conectarlo por WiFi como cliente TCP hacia la Raspberry Pi  para que reciba el comando START, frecuencia, rango, cantidad, modo desde la Raspberry, cambie los registros  del BMI270 antes de cada sesión y ajuste la fórmula de conversión  según el rango elegido



2\. Ahora necesitamos que el ESP32 soporte los dos modos que pide la tarea: SECUENCIAL (donde lee el sensor por I2C y envía por socket de forma bloqueante en un mismo ciclo) y `PARALELO`  y los dos núcleos del ESP32-S3 para que la lectura I2C y el envío por WiFi no se bloqueen mutuamente. Además, separa la parte de WiFi y sockets en tcp\_cliente.c y tcp\_cliente.h.



3\. Aquí tienes nuestros archivos actuales. Actualmente todo funciona por TCP, pero la tarea pide que desde la GUI de la Raspberry Pi se pueda cambiar entre protocolo TCP y UDP en cualquier momento (con TCP por defecto al iniciar) sin reescribir código, y poder comparar el tráfico TCP vs UDP. Modifica los archivos para agregar la selección de TCP/UDP en la GUI, el envío por UDP desde el ESP32 y una tabla comparativa en la interfaz.



4\. Oye necesito que me ayudes a terminar esta tarea, estas son las cosas que debemos agregar por es lo que me falta, asi que necesito que hagas esos cambios.

Esto hay que integrar:

Agregar una opcion en la interfaz para ver la grafica sea continua (hasta el inf)  y que sea con tiempo, ademas de las opciones que ya tenemos.

agregar forma UDP y comparar con TCP (es que ya tenemos).

opción de elegir en la interfaz para ver en la grafica la aceleración o giroscopio.

cambiar la interfaz y que se vea mas moderno



5\. okey ahora quiero que me ayudes a entender lo que yo vere en la interfaz y que es o como se diferencia.

lo de paralelo o secuencial, las muestras, TCP o UDP, la lectura I2C, sevidor sockets y dime tambien sobre la tabla de comparacion, que es lo que compara y como saca esos datos



6\. Al ejecutar el script en la Raspberry Pi me salió un error en la terminal,   tkinter.TclError: bad stickiness value "center":



7\. Probé la interfaz nueva y en modo por muestras funciona bien de momento, pero cuando pongo modo continuo (Inf) o por tiempo se detiene todo con 0 muestras guardadas, o cuando pongo por ejemplo 10 segundos el contador de tiempo salta directo de 0 a 10 segundos en la primera muestra y se corta. Revisa por qué pasa eso y arréglalo en el código de la Raspberry.



8\. Explícame cómo funciona la comunicación I2C entre el ESP32 y el sensor BMI270.



9\. Cual es la diferencia entren los rangos de 2g, 4g y 8g en el acelerómetro



10\. Explícame cual es la diferencia entre TCP y UDP





11\. Oye debemos hacer el informe. Entonces necesito que tú me mandes un zip en LaTeX, el informe debe de ser de 5 a 6 hojas y que tenga la info que tenemos aca



12\. Ayúdame a mejorar la redacción de la conclusión del informe.



13\. Revisa si la explicación de los resultados TCP y UDP coincide con los datos de nuestras pruebas.



