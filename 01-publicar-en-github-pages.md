---
layout: default
title: Práctica 1: Sistemas Embebidos
nav_order: 2
---

# Parte 1: Uso de diferentes microcontroladores

En esta parte, programamos diferentes microcontroladores con el objetivo de aprender a utilizarlos, conocer sus distintas interfaces de programación y desarrollar un programa sencillo para hacer parpadear el **LED integrado** de cada uno:

## 1) ESP32 DevKit V1

### 1.1 Imagen del micro utilizado
En este apartado se muestra una fotografía del **ESP32 DevKit V1** que se utilizó, para identificar claramente el modelo y sus componentes principales (pines, puerto USB, etc.).

![Figura 1 — GitHub](assets/img/01-publicar/Esp32.jpeg)
**Figura 1:** ESP32 DevKit V1.

### 1.2 Imagen de la configuración del intérprete
Aquí se incluye una captura de pantalla de la **configuración del entorno/intérprete** (por ejemplo, Arduino IDE, PlatformIO, Thonny, u otro), mostrando parámetros clave como la placa seleccionada, el puerto COM, la velocidad de carga y/o el intérprete usado.



### 1.3 Imagen del diagrama/conexión del programa
En esta sección se presenta una imagen del **diagrama de conexión o esquema** (si aplica). Para el caso del LED integrado, normalmente no se requiere cableado externo, pero se puede incluir un diagrama que indique el pin del LED o una nota de que es interno.


### 1.4 Programa
A continuación se muestra el **código utilizado** para hacer parpadear el LED integrado. Este programa permite verificar que la placa fue detectada correctamente y que la carga/ejecución funciona como se espera.

```python
# blink_esp32_devkitv1.py
from machine import Pin
import time

LED = Pin(2, Pin.OUT)

while True:
    LED.value(1)
    time.sleep(0.5)
    LED.value(0)
    time.sleep(0.5)