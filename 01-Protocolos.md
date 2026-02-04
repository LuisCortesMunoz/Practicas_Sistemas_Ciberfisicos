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

### 1.4 Registro de datos en terminal

En la Figura 4 se presenta la salida de la terminal durante la ejecución del sistema de medición, en la cual se muestran de manera secuencial el índice del mensaje, la latencia de transmisión y un valor binario que indica la correcta recepción por parte del microcontrolador esclavo. La información fue guardada automáticamente en un archivo CSV para su análisis estadístico.

![Figura 4 — GitHub](assets/img/01-publicar/terminalMaster.jpeg)
*Figura 4:* Datos registrados en terminal.

### 1.5 Resultados de latencia UART

En la Figura 5 se muestra la gráfica de latencia obtenida durante el envío de 1000 mensajes utilizando el protocolo UART. Cada punto representa el tiempo de transmisión de un mensaje individual, mientras que las líneas punteadas indican el valor promedio y una desviación estándar positiva y negativa. Se observa un comportamiento alrededor de 0.65 ms.

![Figura 5 — GitHub](assets/img/01-publicar/Grafica.jpeg)
*Figura 5:* Gráfica de latencia UART.
---

