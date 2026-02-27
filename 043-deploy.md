---
layout: default
title: Deploy
parent: Servicio Web
nav_order: 3
permalink: /servicio-web/deploy/
---

# Práctica 3: Deploy

## 1) Servidor Flask Local

### 1.1) Desarrollo inicial del servidor Flask en entorno local
El primer paso consiste en crear y organizar todos los archivos de código que se utilizarán para ejecutar el servidor Flask de forma local. Esto permite validar que las rutas, las solicitudes (GET/POST) y la lógica general funcionen correctamente antes de desplegar el servicio en la nube (por ejemplo, en Render). En la Figura 1 se muestra la estructura correcta de los archivos y carpetas que se explicarán a continuación.

![Figura 1 — LiveServer](assets/img/01-publicar/orgFlaskLocal.png)

*Figura 1: Estructura de carpetas y archivos del proyecto (backend en Flask y frontend en static) utilizada para el control de la tira de LEDs.*

### 1.2) Servidor Flask local para control de LEDs
Este programa crea un servidor Flask local que sirve la página web desde la carpeta static y expone una API para controlar el estado de la tira LED. El estado (color en formato #RRGGBB, cantidad de LEDs encendidos, rev y updated_at) se guarda en data/state.json para que no se pierda al reiniciar. Para evitar archivos corruptos, usa atomic_write() que escribe primero en un archivo temporal y luego lo reemplaza de forma segura. La ruta GET /api/state devuelve el estado actual y POST /api/state lo actualiza validando que el color y el rango de LEDs sean correctos. Finalmente, corre en 0.0.0.0:5500 para que puedas acceder desde el celular o el ESP32 en la misma red. El archivo app.py se presenta a continuación y está disponible para su descarga.

###  Descargar
[Descargar código Python (app.py)]({{ site.baseurl }}/assets/files/app.py)

### 1.3) Frontend del panel de control (HTML, CSS y JavaScript) servido desde la carpeta static
Estos archivos corresponden al frontend que Flask sirve desde la carpeta static. El archivo index.html define la estructura del panel de control (título, botones y selector de color) que se muestra en el navegador. styles.css se encarga del diseño visual, como colores de fondo, tamaños, espaciado, estilo de botones y el área de mensajes, para que la interfaz sea clara y fácil de usar. Finalmente, app.js contiene la lógica: detecta los clics en All ON / All OFF y en cada LED, guarda el estado seleccionado y envía solicitudes HTTP (POST) a los endpoints de Flask para actualizar el color y qué LEDs deben encenderse, además, puede mandar automáticamente los cambios cuando el usuario modifica el color, logrando una respuesta más inmediata en la tira. A continuación se presenta la carpeta con los archivos previamente explicados (.html, .css y .js), listos para su descarga.

###  Descargar
[Descargar códigos Frontend]({{site.baseurl }}/assets/files/static.zip)

### 1.4)  Persistencia y registro del sistema: estado actual (state.json) e historial de cambios (history.jsonl)
La carpeta data se utiliza para persistir información del sistema, es decir, para que no se pierda cuando el servidor Flask se reinicia. El archivo state.json guarda el estado actual que debe aplicar el ESP32, como el color seleccionado, cuántos LEDs deben estar encendidos (o el arreglo de LEDs, según tu versión), el contador de cambios (rev) y la marca de tiempo (updated_at). Por otro lado, history.jsonl funciona como un registro histórico en formato “JSON por línea”, donde cada actualización puede guardarse como un evento independiente (por ejemplo: fecha/hora, IP del usuario, color y acción), lo que permite auditar cambios, depurar errores y analizar cómo se ha utilizado el panel a lo largo del tiempo. A continuación, se encuentra disponible para descarga la carpeta con los archivos JSON y JSONL.

###  Descargar
[Descargar códigos Backend]({{site.baseurl }}/assets/files/state.zip)

### 1.5)  Configuración del ESP32 en Arduino para sincronización con Flask y control de la tira WS2812 vía Wi-Fi
El último paso antes de obtener los resultados consiste en programar el ESP32 en Arduino para que se conecte a la red Wi-Fi y consulte al servidor Flask el estado más reciente de la tira LED. En este código, lo más importante es configurar correctamente las credenciales de red (WIFI_SSID y WIFI_PASS) y la dirección del servidor donde corre Flask, es decir, la IP de la compu y se puede vualizar con ipconfig en la terminal y el puerto (SERVER_PORT, en este caso 5500). Una vez conectado, el ESP32 realiza una solicitud periódica a la ruta /api/state, recibe un JSON con el color (#RRGGBB), la cantidad de LEDs a encender (count) y un contador de cambios (rev). Con esta información, el microcontrolador convierte el color hexadecimal a valores RGB, actualiza los primeros count LEDs de la tira WS2812 (apagando el resto) y únicamente aplica cambios cuando detecta un rev nuevo, evitando actualizaciones innecesarias. De esta forma, el hardware queda sincronizado con el panel web y refleja en tiempo real los comandos enviados al backend. A continuación se encuentra disponible el código en Arduino para programar el ESP32 y controlar el encendido de la tira de LEDs.

###  Descargar
[Descargar código Arduino (localFlask.ino)]({{ site.baseurl }}/assets/files/localFlask.ino)

### 1.6) Ejecución local del servidor y acceso al panel web
Para finalizar, después de cargar el código en el ESP32, se debe ejecutar el servidor Flask desde Visual Studio Code (corriendo el archivo app.py). Una vez iniciado, el servidor mostrará en consola la dirección donde está disponible el panel web. En este caso, la página se abre desde el navegador ingresando la siguiente URL: http://172.22.26.170:5500. Desde ahí podrás usar la interfaz para enviar comandos y ver reflejados los cambios en la tira de LEDs. Los resultados se muestran en el video: 

### Video
<video controls width="720">
  <source src="{{ '/assets/videos/videoLocalFlask.mp4' | relative_url }}" type="video/mp4">
  Tu navegador no soporta video HTML5.
</video>

## 2) Servidor Flask + Servidor Front (Html)

### 2.1)

## 3) Docker Flask (Servidor Render)

### 3.1)

### 3.2) Frontend: Carpeta con los archivos HTML + CSS + JS

[Descargar códigos Frontend]({{site.baseurl }}/assets/files/tiraLeds.zip)

### 3.3) Backend: Carpeta con los archivos de Docker Flask

[Descargar códigos Backend]({{site.baseurl }}/assets/files/led-git-front-flask-back-docker--main.zip)