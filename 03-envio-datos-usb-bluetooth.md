---
layout: default
title: Interfaz de Usuario
nav_order: 4
---

# Interfaz de Usuario (Python USB / Bluetooth)

En esta práctica se desarrolló un sistema ciberfísico que integra una interfaz gráfica construida en PyQt6 con un dispositivo embebido ESP32 mediante comunicación Bluetooth SPP (Serial Port Profile). El sistema permite controlar un LED físico desde una aplicación de escritorio, manteniendo sincronización en tiempo real entre el mundo digital y el mundo físico.

El objetivo principal fue implementar:

- Una interfaz gráfica funcional  
- Comunicación serial inalámbrica  
- Control bidireccional  
- Visualización del estado del sistema  
- Integración hardware–software  

---

# 1) Instalación del entorno

El primer paso consistió en instalar la librería PyQt6, la cual permite desarrollar interfaces gráficas en Python.

Se abrió la terminal del sistema y se ejecutó:

```bash
pip install PyQt6
````

Posteriormente se verificó la instalación:

```bash
pip show PyQt6
```

Esto confirmó que el framework Qt estaba correctamente disponible para el desarrollo de la aplicación.

---

# 2) Desarrollo de la interfaz gráfica

La interfaz fue programada en Visual Studio Code utilizando Python y la librería PyQt6.

PyQt6 permite crear aplicaciones gráficas mediante widgets como:

* Botones
* Ventanas
* Etiquetas
* Contenedores
* Indicadores visuales
* Elementos personalizados

Se utilizaron los módulos principales:

* QtWidgets
* QtCore
* QtGui
* PySerial

---

## 2.1 Diseño de la interfaz

La aplicación se estructuró en tres secciones principales:

* Conexión Bluetooth
* Control del LED
* Registro de comunicación

![Interfaz PyQt6](assets/img/InterfazPyQt6.png)

**Figura 1:** Interfaz gráfica desarrollada en PyQt6.

---

## 2.2 Comunicación con el ESP32

La aplicación implementa comunicación serial Bluetooth utilizando PySerial, permitiendo:

* Detectar puertos COM
* Conectarse al dispositivo
* Enviar comandos
* Recibir confirmaciones
* Actualizar el estado del LED

---

# 3) Programación del ESP32

El ESP32 fue programado mediante Arduino IDE.

---

## 3.1 Configuración Bluetooth

Se utilizó la librería:

```cpp
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
```

El dispositivo se publicó con el nombre:

```cpp
SerialBT.begin("ESP32_LED");
```

---

## 3.2 Control del LED

El LED interno fue configurado como salida:

```cpp
const int LED_PIN = 2;
pinMode(LED_PIN, OUTPUT);
```

El ESP32 recibe comandos:

```cpp
if (cmd == "LED_ON") {
 digitalWrite(LED_PIN, HIGH);
 SerialBT.println("LED_ON_OK");
}
```

Cada acción genera confirmación, asegurando sincronización con la GUI.

---

# 4) Emparejamiento Bluetooth

Se activó el Bluetooth del sistema y se buscó el dispositivo **ESP32_LED**.

![Emparejamiento Bluetooth](assets/img/PyQt_configuracion.png)

**Figura 2:** Emparejamiento del ESP32.

---

# 5) Identificación del puerto COM

Después del emparejamiento se accedió a:

Configuración → Bluetooth → Más opciones → Puertos COM

Se identificó el puerto saliente asignado al dispositivo.

![Puertos COM](assets/img/configu_COM.png)

**Figura 3:** Identificación del puerto serial Bluetooth.

---

# 6) Ejecución del sistema

La aplicación se ejecutó mediante:

```bash
python nombre_archivo.py
```

Posteriormente:

* Se seleccionó el puerto COM
* Se presionó conectar
* El indicador cambió a verde
* Se habilitó el control del LED

El sistema permitió encender y apagar el LED físico observando simultáneamente el cambio en el LED virtual.

---

# 7) Arquitectura del sistema

```
+------------------+      Bluetooth SPP      +------------------+
|       PC GUI     | <---------------------> |      ESP32       |
|   PyQt6 + Serial |                         | BluetoothSerial  |
+------------------+                         +------------------+
        |                                             |
        |                                             |
   LED virtual                                  LED físico
```

---

# 8) Resultados

El sistema logró establecer comunicación bidireccional estable entre la interfaz gráfica y el ESP32.

La interfaz respondió correctamente al establecer conexión, mostrando el indicador activo y habilitando el control del LED. El registro de eventos permitió observar el intercambio de mensajes entre ambos dispositivos, confirmando la correcta transmisión de datos.

El LED virtual reflejó en tiempo real el estado del LED físico, validando el modelo de confirmación implementado mediante mensajes de retorno.

Los resultados demostraron que:

* La interfaz es funcional
* La comunicación Bluetooth es estable
* El sistema responde en tiempo real
* El control es confiable
* La sincronización es correcta

---

# 9) Errores y Debugging

Durante el desarrollo se presentaron diversos desafíos técnicos.

## Problemas de instalación

Se resolvieron verificando Python y configurando variables de entorno.

## Detección incorrecta del puerto

Se identificó que solo el puerto COM saliente permite comunicación efectiva.

## Fallos de emparejamiento

Se solucionaron reiniciando el dispositivo y repitiendo el proceso de vinculación.

## Bloqueo de la interfaz

Se resolvió utilizando temporizadores para lectura no bloqueante.

## Desincronización del LED

Se implementó confirmación basada en mensajes de retorno.

Estas pruebas permitieron mejorar la robustez del sistema.

---

# 10) Análisis técnico

El sistema desarrollado representa un ejemplo funcional de arquitectura ciberfísica donde se integran:

* Interfaz digital
* Sistema embebido
* Comunicación inalámbrica
* Control en tiempo real
* Retroalimentación

La implementación demuestra la importancia de la confirmación de estados en sistemas distribuidos, así como el uso de protocolos seriales virtuales para aplicaciones IoT y automatización.

---

# 11) Conclusión

Se logró implementar exitosamente una interfaz gráfica en PyQt6 capaz de comunicarse con un sistema embebido mediante Bluetooth, controlando un dispositivo físico en tiempo real. La práctica permitió aplicar conceptos fundamentales de sistemas ciberfísicos, integración hardware–software y comunicación inalámbrica.

El sistema desarrollado demuestra la viabilidad de construir aplicaciones robustas que conectan el mundo digital con el físico, sentando las bases para aplicaciones de automatización, monitoreo remoto e Internet de las Cosas.

---

# 12) Posibles mejoras

* Control de múltiples dispositivos
* Implementación BLE
* Interfaz multiplataforma
* Registro de datos
* Control PWM
* Aplicación ejecutable
* Reconexión automática
