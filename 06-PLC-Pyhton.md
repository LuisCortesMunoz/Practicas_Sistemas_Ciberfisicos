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

## 4.

[Descargar programa escalera]({{site.baseurl }}/assets/files/practicaPLC.zip)

## 5.