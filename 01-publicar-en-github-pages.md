---
layout: default
title: Sistemas Embebidos
nav_order: 2
has_children: true
permalink: /sistemas-embebidos/
---

# Sistemas Embebidos

En esta sección se documentan las prácticas relacionadas con **sistemas embebidos** (microcontroladores), desde cómo se programan y configuran, hasta el diseño y fabricación de una PCB, así como la implementación de protocolos de comunicación y otras aplicaciones prácticas.

## 5) XIAO RP2040

### 5.1 Imagen del micro utilizado

En este apartado se muestra una fotografía del *XIAO RP2040* que se utilizó, para identificar claramente el modelo y sus componentes principales (pines, puerto, etc.).

![Figura 10 — GitHub](assets/img/01-publicar/RP20240.jpg)
*Figura 10:* XIAO RP2040.

---

### 5.2 Imagen de la configuración del intérprete

Aquí se incluye una captura de pantalla de la configuración del entorno/intérprete utilizado (en este caso, **Thonny**), mostrando parámetros clave como el tipo de placa, el puerto y la versión del intérprete.

![Figura 11 — GitHub](assets/img/01-publicar/interpreteRP2040.jpeg)
*Figura 11:* Thonny MicroPython (XIAO RP2040).

---

### 5.3 Video

En el siguiente enlace se puede visualizar el video del funcionamiento del XIAO RP20240, donde se observa el parpadeo del LED integrado:

<video controls width="720">
  <source src="{{ 'assets/videos/blinkRP2040.mp4' | relative_url }}" type="video/mp4">
  Tu navegador no soporta video HTML5.
</video>

### 5.4 Programa

A continuación se muestra el *código utilizado* para hacer parpadear el LED integrado. Este programa permite verificar que la placa fue detectada correctamente y que la carga/ejecución funciona como se espera.

```python
# blink_xiao_rp2040.py
from machine import Pin
import time

LED = Pin(17, Pin.OUT)  # activo-bajo

while True:
    LED.value(0)  # ON
    time.sleep(0.5)
    LED.value(1)  # OFF
    time.sleep(0.5)
```

---

### 5.5 Nota adicional

Antes de conectar la XIAO RP2040 para que el equipo la detecte correctamente, es necesario presionar el botón **BOOT** y mantenerlo presionado mientras se conecta el cable USB a la computadora. Esto permite que la placa entre en modo de carga y aparezca como unidad/dispositivo, facilitando su reconocimiento en Thonny.
