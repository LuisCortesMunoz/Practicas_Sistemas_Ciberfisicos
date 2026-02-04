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

### 1.1 Imagen del micro utilizado
Claro — te la dejo más clara, técnica y fluida (sin que suene a IA):

En este apartado se muestra una fotografía de los pines de la *XIAO ESP32-S3*, con el propósito de identificar de manera clara las conexiones utilizadas para el protocolo de comunicación empleado en la práctica.

![Figura 1 — GitHub](assets/img/01-publicar/pinoutXIAO.jpg)
*Figura 1:* Pinout XIAO ESP32-S3.
