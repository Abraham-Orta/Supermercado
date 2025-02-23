SIMULADOR DE SUPERMERCADO CON CARRITOS MÓVILES EN QT

Este proyecto es una simulación gráfica de un supermercado donde carritos de compra se mueven automáticamente a lo largo de una ruta predefinida. Desarrollado con Qt, muestra un entorno interactivo con estantes, caja registradora y funcionalidades para generar carritos y visualizar facturas.

CARACTERÍSTICAS PRINCIPALES
- Simulación de movimiento de carritos con ruta predefinida
- Interfaz gráfica con elementos de supermercado:
  * Suelo texturizado
  * Estantes y caja registradora
  * Botones de control
- Sistema de generación automática de carritos
- Visualizador de facturas desde archivo .txt
- Animación fluida con actualizaciones cada 10 ms

REQUISITOS
- Qt5 (Core, GUI, Widgets)
- Compilador C++ (g++, clang, etc.)
- Archivos de recursos:
  * Imágenes: carrito.png, suelo1.png, estante.png, caja.png
  * Archivo de factura: factura.txt

INSTALACIÓN
1. Clonar repositorio:
   git clone git@github.com:Abraham-Orta/Supermercado.git
   cd supermercado_proyecto

2. Configurar rutas de imágenes en el código:
   - Modificar las rutas absolutas (/home/pancho/supermercado_proyecto/...) según tu estructura de archivos.

3. Compilar proyecto:
   qmake && make

USO
- Ejecutar programa:
  ./nombre_del_ejecutable

- Controles:
  * Botón "Iniciar Generación": Activa creación de carritos cada 1 segundo
  * Botón "Detener Generación": Pausa la generación
  * Botón "Mostrar Facturas": Abre ventana con contenido de factura.txt

- Comportamiento de carritos:
  * Siguen ruta predefinida automáticamente
  * Se eliminan al llegar al punto final
  * Velocidad ajustable (actualmente 1px/actualización)

ESTRUCTURA DEL CÓDIGO
Clases principales:
- CarritoMovil: Gestiona movimiento y ruta de los carritos
- GeneradorCarritos: Controla creación periódica de carritos
- VentanaFactura: Muestra contenido de archivo de facturas
