---
layout: default
title: PLC con Python
nav_order: 7
---

# Control de un PLC mediante una Interfaz Web en Python

A continuación, se presenta la práctica sobre la conexión de un PLC con una interfaz desarrollada en Python, integrando programación en Python y lenguaje escalera. Se muestra el procedimiento paso a paso para encender un LED del PLC mediante botones alojados en un servicio web desarrollado con Flask.

En esta práctica se utilizó un PLC Allen-Bradley Micro850-LC50-24QWB, versión 12.

## 1. Instalación de herramientas necesarias para la programación del PLC y la interfaz en Python

Primero, es necesario instalar las herramientas requeridas para el desarrollo de la práctica. En este caso, se debe contar con Visual Studio Code o cualquier otro entorno de desarrollo compatible con Python. Posteriormente, para programar el PLC, es necesario descargar e instalar el software Connected Components Workbench (CCW), el cual se utiliza para la programación del PLC Allen-Bradley. El enlace de la página oficial desde donde puede descargarse se muestra a continuación.

[https://www.rockwellautomation.com](https://www.rockwellautomation.com/es-mx/capabilities/industrial-automation-control/design-and-configuration-software.html)

## 2. Configuración inicial del PLC en Connected Components Workbench

Una vez instalado el software, se abre Connected Components Workbench (CCW), donde se mostrará la ventana inicial de configuración. En esta sección, se debe seleccionar el modelo de PLC con el que se está trabajando. Después de elegir la configuración correcta, se da clic en “Agregar al proyecto” como se muestra en la Figura 1. Con este paso, el PLC Allen-Bradley queda listo para comenzar su programación.

![PLC1](assets/img/01-publicar/PLC1.png)
*Figura 1:* Ventana de selección y configuración inicial del PLC en CCW.

## 3. Configuración del mapeo Modbus y habilitación de Ethernet en el PLC

El siguiente paso consiste en configurar las variables de Modbus. Para ello, es necesario dirigirse a la configuración del Micro850 en el panel izquierdo, como se muestra en la Figura 2 dentro del recuadro rojo. Una vez ahí, se debe buscar la opción “Mapeado Modbus” y agregar la variable que se utilizará para enviar la señal desde el servicio web, tal como se indica en el recuadro azul de la Figura 2.

![PLC2](assets/img/01-publicar/PLC2.png)
*Figura 2:* Configuración de variables en el mapeado Modbus del Micro850.

Posteriormente, en la configuración de Ethernet, se debe habilitar el mapeado Modbus, como se muestra en la Figura 3 dentro del recuadro rojo.

![PLC3](assets/img/01-publicar/PLC3.png)
*Figura 3:* Habilitación del mapeado Modbus en la configuración Ethernet del PLC.

## 4. Creación del programa en lenguaje escalera para el control de una lámpara

Como tercer paso, se debe crear el programa en lenguaje escalera (Ladder Diagram). En este caso, se desarrolló un programa sencillo para accionar una lámpara del PLC mediante un botón ubicado en una interfaz web.

Primero, como se muestra en la Figura 4, se debe dar clic derecho en la opción “Programas”, después seleccionar “Agregar” y, por último, “Nuevo LD: Diagrama en escalera”. 

![PLC4](assets/img/01-publicar/PLC4.png)
*Figura 4:* Creación de un nuevo programa Ladder en CCW.

Con esto, el software redireccionará a la pestaña mostrada en la Figura 5, donde se puede crear el programa que se desea implementar.

En esta práctica, se realizó una lógica simple en la que, cuando la variable de entrada —creada previamente en el mapeado Modbus— es igual a 1, se activa la salida 2, la cual está conectada a la lámpara en el panel de control.

![PLC5](assets/img/01-publicar/PLC5.png)
*Figura 5:* Diseño del programa en lenguaje escalera para el control de la lámpara.

Finalmente, se muestra a continuación la carpeta que contiene el programa completo con toda la configuración descrita, para su descarga.

[Descargar programa escalera]({{site.baseurl }}/assets/files/practicaPLC.zip)

## 5.

### 1.1) app.py

```python
# Step 1: Import libraries
from flask import Flask, request, render_template_string
from pymodbus.client import ModbusTcpClient

# Step 2: PLC configuration
PLC_IP = "192.168.3.152"
PLC_PORT = 502
REGISTER_ADDRESS = 0

# Step 3: Create Flask app
app = Flask(__name__)

# Step 4: Function to write to PLC
def write_value(val):
    client = ModbusTcpClient(PLC_IP, port=PLC_PORT)

    try:
        if client.connect():
            result = client.write_register(REGISTER_ADDRESS, val)

            if result.isError():
                return False, f"Error Modbus: {result}"

            return True, f"Registro escrito con valor {val}"
        else:
            return False, "No se pudo conectar al PLC"

    except Exception as e:
        return False, f"Excepción: {e}"

    finally:
        client.close()

# Step 5: Web page
HTML_PAGE = """
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>PLC LED Control</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin-top: 60px;
            background-color: #f4f4f4;
        }
        h1 {
            margin-bottom: 30px;
        }
        button {
            width: 180px;
            height: 60px;
            font-size: 20px;
            margin: 10px;
            border: none;
            border-radius: 10px;
            cursor: pointer;
        }
        .on {
            background-color: green;
            color: white;
        }
        .off {
            background-color: red;
            color: white;
        }
        .msg {
            margin-top: 25px;
            font-size: 18px;
            color: blue;
        }
    </style>
</head>
<body>
    <h1>Control Web del PLC</h1>

    <form action="/on" method="post">
        <button class="on" type="submit">Turn ON</button>
    </form>

    <form action="/off" method="post">
        <button class="off" type="submit">Turn OFF</button>
    </form>

    <div class="msg">{{ message }}</div>
</body>
</html>
"""

# Step 6: Main page
@app.route("/", methods=["GET"])
def home():
    return render_template_string(HTML_PAGE, message="Listo para controlar el PLC")

# Step 7: Route to turn ON
@app.route("/on", methods=["POST"])
def turn_on():
    ok, msg = write_value(1)
    return render_template_string(HTML_PAGE, message=msg)

# Step 8: Route to turn OFF
@app.route("/off", methods=["POST"])
def turn_off():
    ok, msg = write_value(0)
    return render_template_string(HTML_PAGE, message=msg)

# Step 9: Run server
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=False)
```