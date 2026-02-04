---
layout: default
title: Práctica 3
parent: Sistemas Embebidos
nav_order: 3
permalink: /sistemas-embebidos/practica-3/
---

# Práctica 3: Protocolos de comunicación

En esta sección se realizó la implementación y evaluación de los protocolos de comunicación UART, I2C y SPI utilizando tres plataformas de sistemas embebidos: ATMEGA328, ESP32-S3 y RP2040. Para cada plataforma se configuraron los periféricos de comunicación correspondientes y se estableció la conexión entre dos dispositivos con el fin de transmitir datos de forma controlada. Posteriormente, se enviaron 1000 mensajes por cada protocolo para medir la latencia de comunicación, registrando los tiempos de transmisión y generando gráficas de comportamiento. Finalmente, se compararon los resultados obtenidos entre protocolos y plataformas, analizando el desempeño en términos de velocidad, estabilidad y tiempo de respuesta, e integrando en el reporte los diagramas de arquitectura, esquemas eléctricos y códigos utilizados.

## 1) Serial (UART)

UART (universal asynchronous receiver / transmitter, por sus siglas en inglés) se define como un protocolo para el intercambio de datos en serie entre dos dispositivos. UART utiliza solo dos hilos entre el transmisor y el receptor para transmitir y recibir en ambas direcciones. Ambos extremos tienen una conexión a masa. La comunicación en UART puede ser simplex(los datos se envían en una sola dirección), semidúplex(cada extremo se comunica, pero solo uno al mismo tiempo), o dúplex completo(ambos extremos pueden transmitir simultáneamente).

### 1.1 Imagen del microontrolador utilizados

En este apartado se muestra una fotografía de los pines de la *XIAO ESP32-S3 y Arduino UNO*, con el propósito de identificar de manera clara las conexiones utilizadas para el protocolo de comunicación empleado en la práctica.

![Figura 1 — GitHub](assets/img/01-publicar/pinoutXIAO.jpg)
*Figura 1:* Pinout XIAO ESP32-S3.

![Figura 2 — GitHub](assets/img/01-publicar/pinoutUNO.jpg)
*Figura 2:* Pinout Arduino UNO.

### 1.2 Imagen de conexíon entre microontroladores 

En este apartado se muestra una fotografía de la conexión para el protocolo UART entre una XIAO ESP32-S3 como maestro y como esclavo Arduino UNO.

![Figura 3 — GitHub](assets/img/01-publicar/conexionFisica.jpeg)
*Figura 3:* Conexión física.

### 1.3 Código utilizado

En el maestro se construye una trama de 6 bytes: byte de inicio, secuencia y checksum XOR; se envía por UART y se mide el tiempo hasta recibir el eco completo.

````md
```Micro python
START_BYTE = 0xA5

def make_frame(k: int) -> bytes:
    seq = struct.pack("<I", k)
    first5 = bytes([START_BYTE]) + seq
    cs = 0
    for bb in first5:
        cs ^= bb
    return first5 + bytes([cs])
```
````

En el esclavo se recibe la trama, se verifica el checksum y, si es válida, se responde con los mismos 6 bytes.


````md
```C++
const uint8_t START_BYTE = 0xA5;

uint8_t checksum5(const uint8_t *p) {
  uint8_t c = 0;
  for (int i = 0; i < 5; i++) c ^= p[i];
  return c;
}
```
````
### 1.4 Registro de datos en terminal

En la Figura 4 se presenta la salida de la terminal durante la ejecución del sistema de medición, en la cual se muestran de manera secuencial el índice del mensaje, la latencia de transmisión y un valor binario que indica la correcta recepción por parte del microcontrolador esclavo. La información fue guardada automáticamente en un archivo CSV para su análisis estadístico.

![Figura 4 — GitHub](assets/img/01-publicar/terminalMaster.jpeg)
*Figura 4:* Datos registrados en terminal.


### 1.5 Resultados de latencia UART

En la Figura 5 se muestra la gráfica de latencia obtenida durante el envío de 1000 mensajes utilizando el protocolo UART. Cada punto representa el tiempo de transmisión de un mensaje individual, mientras que las líneas punteadas indican el valor promedio y una desviación estándar positiva y negativa. Se observa un comportamiento alrededor de 0.65 ms.

![Figura 5 — GitHub](assets/img/01-publicar/Grafica.jpeg)
*Figura 5:* Gráfica de latencia UART.
---

## 2) Interfaz periférica en serie (SPI) 
Serial Peripheral Interface (SPI) es un protocolo de comunicación serie síncrono utilizado para permitir el intercambio de datos a alta velocidad entre un dispositivo maestro y varios dispositivos periféricos. Se trata de un sistema de comunicación full-duplex diseñado para transferir datos de forma rápida y eficaz en distancias cortas.

### 2.1 Imagen de conexíon entre microontroladores 

En este apartado se muestra una fotografía de la conexión para el protocolo SPI entre una XIAO ESP32-S3 como maestro y como esclavo Arduino UNO.

![Figura 6 — GitHub](assets/img/01-publicar/conexiSPI.jpeg)
*Figura 6:* Conexión física.