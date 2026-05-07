---
layout: default
title: Servicios de almacenamiento en la nube
nav_order: 6
---

# ESP32 con botón, servidor Flask y Firebase

## Objetivo de la práctica

El objetivo de esta práctica fue implementar una comunicación entre una **ESP32** y un **servidor desarrollado en Flask**, para que la ESP32 pudiera enviar información mediante una petición **HTTP POST** cuando se presionara un botón físico. Posteriormente, el servidor Flask recibiría esa información en formato **JSON** y la enviaría a una base de datos en la nube usando **Firebase Firestore**.

La idea general fue simular un sistema de monitoreo remoto, donde un dispositivo físico pudiera reportar un evento hacia un servidor local y después almacenar ese evento en una base de datos en la nube para su consulta posterior.

---

## Descripción general del sistema

El sistema se compone de tres partes principales:

1. Una **ESP32** conectada a un botón físico.
2. Un **servidor Flask** ejecutándose en una computadora.
3. Una **base de datos en Firebase Firestore**, utilizada para guardar los eventos recibidos.

La ESP32 se conecta a una red WiFi y permanece revisando constantemente el estado de un botón conectado al pin **GPIO 4**. Cuando el botón es presionado, la ESP32 genera un mensaje en formato JSON y lo envía al servidor Flask mediante una petición HTTP POST.

El servidor Flask recibe esa petición en el endpoint `/evento`, interpreta el contenido del JSON y guarda la información recibida en una colección de Firebase Firestore llamada `eventos`.

---

## Flujo general de funcionamiento

El funcionamiento general del sistema es el siguiente:

```text
Botón físico
    ↓
ESP32 detecta que el botón fue presionado
    ↓
ESP32 genera un JSON con el evento
    ↓
ESP32 realiza una petición HTTP POST al servidor Flask
    ↓
Flask recibe el JSON en el endpoint /evento
    ↓
Flask guarda el evento en Firebase Firestore
    ↓
El evento queda disponible en la nube para consulta posterior