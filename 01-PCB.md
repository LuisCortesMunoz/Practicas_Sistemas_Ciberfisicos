---
layout: default
title: Práctica 2
parent: Sistemas Embebidos
nav_order: 2
permalink: /sistemas-embebidos/practica-2/
---

# Práctica 2: Diseño y elaboración de PCB.

En esta sección realizamos el diseño de una PCB en KiCAD y preparamos los archivos de fabricación en FabLab. Posteriormente, generamos el archivo de grabado (toolpaths) para poder maquinar la placa en una MonoFab SRM-20 Desktop Milling Machine, asegurando las dimensiones, el ruteo y la configuración correcta antes del fresado.

## 1) Instalación y configuración del complemento FabLab en KiCAD

Primero, se debe descargar e instalar **KiCAD**. Al abrir el programa, se mostrará una ventana inicial (ver **Figura 1**). En esta etapa, el siguiente paso es configurar el complemento necesario para trabajar con FabLab:

1. Haz clic en Gestor de complementos.  
2. En la ventana que aparece, selecciona la pestaña Instalados (en la parte superior).  
3. Busca e instala el complemento KiCAD FabLab.  
4. Finalmente, presiona el botón Actualizar** para aplicar los cambios y, al terminar, cierra la ventana del gestor.


![Figura 1 — GitHub](assets/img/01-publicar/pcb_paso1.png)
*Figura 1:* Pantalla inicial de configuración en el software KiCAD.

## 2) Creación de un nuevo proyecto y acceso al editor esquemático en KiCAD

El siguiente paso es crear un nuevo proyecto. Desde la misma ventana inicial (ver Figura 1), ve al menú Archivo y selecciona Nuevo proyecto. Después, guarda el proyecto en la ruta que prefieras.

Una vez creado, el proyecto aparecerá en la barra lateral izquierda. Para comenzar con el diseño, haz clic en Esquemático, como se muestra en la Figura 2 (recuadro rojo).

![Figura 2 — GitHub](assets/img/01-publicar/pcb_paso2.png)
*Figura 2:* Creación de un nuevo proyecto.

## 3) Búsqueda e inserción de componentes en el editor esquemático

Como tercer paso, se mostrará una ventana similar a la de la Figura 3 (nota: al abrirla no se visualizarán los componentes como en la imagen de ejemplo). Para buscar e insertar diferentes componentes, utiliza la barra de herramientas del lado izquierdo. En la Figura 3 (flecha y recuadro rojo), debes dar clic en el icono “Place symbols (A)”.

![Figura 3 — GitHub](assets/img/01-publicar/pcb_paso3.png)
*Figura 3:* Barra de herramientas del editor esquemático y acceso a “Place symbols (A)”.

Al seleccionarlo, se abrirá una ventana como la que se muestra en la Figura 4. Ahí podrás buscar el componente que necesitas. Al elegirlo, en el lado derecho (recuadro azul) se mostrará la vista previa del símbolo para verificar que sea el correcto. Cuando lo tengas identificado, presiona el botón “Aceptar” en la parte inferior.

![Figura 4 — GitHub](assets/img/01-publicar/pcb_paso31.png)
*Figura 4:* Ventana de selección de componentes: búsqueda y vista previa del símbolo.

**Nota**: para este paso es importante que ya tengas definidos previamente los componentes que vas a utilizar.
