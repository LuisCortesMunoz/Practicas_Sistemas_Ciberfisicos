---
layout: default
title: Página Web
parent: Servicio Web
nav_order: 1
permalink: /servicio-web/pagina-web/
---

# Práctica 1: Página Web

## 1) Creación de la página (index.html)

Al momento de elaborar la página web, el primer paso es crear un archivo index.html, el cual funcionará como la portada del sitio. En la sección 1.1 se muestra el código utilizado. Ahí puedes ir construyendo la estructura básica de la página, agregando el encabezado <head> (título) y el cuerpo <body> (contenido visible como textos, imágenes y secciones).

### 1.1 Código index.html

```html
<!doctype html>
<html>
<head>
    <title>Luis CM</title>
</head>
<body>
    <h1>Ing. Luis Cortes</h1>
    <p>Este es mi primer proyecto web con HTML + CSS + JS.</p>
</body>
</html>
```

## 2) Visualización local con Live Server

Posteriormente, se recomienda trabajar en Visual Studio Code (escritorio) e instalar la extensión Live Server, la cual permite visualizar la página en un servidor local (localhost) y actualizarla automáticamente al guardar cambios. Una vez instalada la extensión, abre el archivo index.html, haz clic derecho y selecciona “Open with Live Server”. Si todo está configurado correctamente, se mostrará una pantalla como la presentada en la **Figura 1**.

![Figura 1 — LiveServer](assets/img/01-publicar/paginaWeb1.png)

*Figura 1: Visualización de la página web en localhost mediante la extensión Live Server en Visual Studio Code.*

En la **Figura 1** se observa que, en la parte superior del navegador, la pestaña muestra el título Luis CM, y en la página se visualiza el contenido definido dentro del <body> del archivo index.html.


## 3) json

```json
{
    "liveServer.settings.port":5500,
    "liveServer.settings.host":"localhost"
}
```

## 4) app

```python
# Step 1: Imports
from flask import Flask, request
from datetime import datetime

# Step 2: Create app
app = Flask(__name__)

# Step 3: File name
FILE_NAME = "datos.txt"

# Step 4: Get client IP
def get_ip():
    forwarded = request.headers.get("X-Forwarded-For", "")
    if forwarded:
        return forwarded.split(",")[0].strip()
    return request.remote_addr or "unknown"

# Step 5: Append one line to txt (CSV style)
def append_line(line):
    with open(FILE_NAME, "a", encoding="utf-8") as f:
        f.write(line + "\n")

# Step 6: Read full txt
def read_lines():
    try:
        with open(FILE_NAME, "r", encoding="utf-8") as f:
            return f.read()
    except FileNotFoundError:
        return ""

# Step 7: Save by URL: /guardar?dato=hola
@app.get("/guardar")
def guardar():
    # Step 7.1: Read query param
    dato = (request.args.get("dato") or "").strip()
    if dato == "":
        return "Error: usa /guardar?dato=algo", 400

    # Step 7.2: Build fields
    ip = get_ip()
    hora = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    # Step 7.3: Save as one line: dato,ip,hora
    append_line(f"{ip}; {hora}; {dato}")

    return f"Guardado ✅: {ip};{hora};{dato}", 200

# Step 8: Read: /leer
@app.get("/leer")
def leer():
    text = read_lines()
    return text, 200, {"Content-Type": "text/plain; charset=utf-8"}

# Step 9: Run
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
```


 hola http://172.22.26.148:5000/guardar?dato=hola se agregue el dato 

y cuando haga http://172.22.26.148:5000/leer se lea