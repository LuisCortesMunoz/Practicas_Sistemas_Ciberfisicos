---
layout: default
title: Página Web
parent: Servicio Web
nav_order: 1
permalink: /servicio-web/pagina-web/
---

# Práctica 1: Página Web

```HTML
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

```Python
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

```json
{
    "liveServer.settings.port":5500,
    "liveServer.settings.host":"localhost"
}
```