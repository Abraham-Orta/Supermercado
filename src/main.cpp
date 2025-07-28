// Incluye las bibliotecas necesarias para el programa
#include <QApplication> // Para la aplicación Qt
#include <QGraphicsScene> // Para manejar la escena gráfica
#include <QGraphicsView> // Para mostrar la escena gráfica
#include <QGraphicsPixmapItem> // Para manejar imágenes en la escena
#include <QTimer> // Para manejar temporizadores
#include <QList> // Para usar listas
#include <QPointF> // Para manejar puntos en coordenadas flotantes
#include <QLineF> // Para manejar líneas en coordenadas flotantes
#include <QDebug> // Para imprimir mensajes de depuración
#include <QWidget> // Para crear ventanas y widgets
#include <QPushButton> // Para crear botones
#include <QVBoxLayout> // Para organizar widgets verticalmente
#include <QHBoxLayout> // Para organizar widgets horizontalmente
#include <QDialog> // Para crear ventanas de diálogo
#include <QLabel> // Para mostrar texto en la interfaz
#include <QFile> // Para manejar archivos
#include <QTextStream> // Para leer y escribir archivos de texto
#include <QMessageBox> // Para mostrar mensajes emergentes
#include <QTextEdit> // Para mostrar y editar texto
#include <QStringList> // Para manejar listas de cadenas
#include <QMap> // Para manejar mapas (diccionarios)
#include <QSplitter> // Para dividir ventanas en secciones
#include <QRandomGenerator> // Para generar números aleatorios
#include <QString> // Para manejar cadenas de texto
#include <vector> // Para usar vectores (listas dinámicas)
#include <random> // Para generar números aleatorios
#include <sstream> // Para manipular cadenas de texto
#include <QGroupBox> // Para agrupar widgets
#include <QTableWidget> // Para mostrar tablas
#include <QHeaderView> // Para personalizar encabezados de tablas
#include <QIcon> // Para manejar íconos
#include <QFont> // Para manejar fuentes de texto

// Declaración de variables globales
std::vector<std::string> cedulasgeneradas; // Almacena cédulas generadas para evitar duplicados
std::vector<std::string> telefonogeneradas; // Almacena teléfonos generados para evitar duplicados

// Función para generar un nombre aleatorio
std::string GenerarNombre() {
    std::vector<std::string> nombres = {"Juan", "María", "Carlos", "Ana", "Luis", "Alejandro"}; // Lista de nombres
    std::vector<std::string> apellidos = {"Pérez", "Gómez", "López", "Martínez", "Rodríguez", "Rivas", "Sifontes"}; // Lista de apellidos

    std::random_device rd; // Generador de números aleatorios
    std::mt19937 gen(rd()); // Motor de números aleatorios
    std::uniform_int_distribution<> distribNombre(0, nombres.size() - 1); // Distribución para nombres
    std::uniform_int_distribution<> distribApellido(0, apellidos.size() - 1); // Distribución para apellidos

    // Combina un nombre y un apellido aleatorios
    std::string nombreCompleto = nombres[distribNombre(gen)] + " " + apellidos[distribApellido(gen)];
    return nombreCompleto; // Retorna el nombre completo
}

// Función para generar una cédula aleatoria
std::string GenerarCedula() {
    std::random_device rd; // Generador de números aleatorios
    std::mt19937 gen(rd()); // Motor de números aleatorios
    std::uniform_int_distribution<int> distrib(4000000, 40000000); // Rango para la cédula

    std::string cedula; // Almacena la cédula generada
    bool Duplicada; // Indica si la cédula ya existe

    do {
        int cedulaNum = distrib(gen); // Genera un número aleatorio
        cedula = std::to_string(cedulaNum); // Convierte el número a cadena
        // Verifica si la cédula ya fue generada
        Duplicada = (std::find(cedulasgeneradas.begin(), cedulasgeneradas.end(), cedula) != cedulasgeneradas.end());
    } while (Duplicada); // Repite si la cédula está duplicada

    cedulasgeneradas.push_back(cedula); // Almacena la cédula generada
    return cedulasgeneradas.back(); // Retorna la cédula
}

// Función para generar un teléfono aleatorio
std::string GenerarTelefono() {
    std::random_device rd; // Generador de números aleatorios
    std::mt19937 gen(rd()); // Motor de números aleatorios
    std::uniform_int_distribution<int> distrib1(500000, 40000000); // Rango para el número de teléfono
    std::uniform_int_distribution<int> distrib2(1, 5); // Rango para el prefijo

    int prefijo; // Almacena el prefijo del teléfono
    std::string telefono; // Almacena el teléfono generado
    bool Duplicada; // Indica si el teléfono ya existe

    do {
        int telefonoNum = distrib1(gen); // Genera un número aleatorio
        prefijo = distrib2(gen); // Genera un prefijo aleatorio
        std::stringstream completo; // Para construir el teléfono completo

        // Asigna el prefijo según el número generado
        switch (prefijo) {
        case 1:
            completo << "0424" << telefonoNum; // Prefijo 0424
            break;
        case 2:
            completo << "0414" << telefonoNum; // Prefijo 0414
            break;
        case 3:
            completo << "0412" << telefonoNum; // Prefijo 0412
            break;
        case 4:
            completo << "0416" << telefonoNum; // Prefijo 0416
            break;
        case 5:
            completo << "0426" << telefonoNum; // Prefijo 0426
            break;
        }

        telefono = completo.str(); // Convierte el teléfono a cadena
        // Verifica si el teléfono ya fue generado
        Duplicada = (std::find(telefonogeneradas.begin(), telefonogeneradas.end(), telefono) != telefonogeneradas.end());
    } while (Duplicada); // Repite si el teléfono está duplicado

    telefonogeneradas.push_back(telefono); // Almacena el teléfono generado
    return telefonogeneradas.back(); // Retorna el teléfono
}

// Clase que representa a un cliente
class Cliente {
public:
    Cliente() : id(0), nombre(""), estado("") {} // Constructor por defecto
    Cliente(int id, const QString &nombre, const QString &estado, const QMap<QString, int> &listaCompra = QMap<QString, int>())
        : id(id), nombre(nombre), estado(estado), listaCompra(listaCompra) {} // Constructor con parámetros

    // Métodos para obtener los atributos del cliente
    int getId() const { return id; }
    QString getNombre() const { return nombre; }
    QString getEstado() const { return estado; }
    QMap<QString, int> getListaCompra() const { return listaCompra; }
    QString getCedula() const { return cedula; }
    QString getTelefono() const { return telefono; }

    // Método para cambiar el estado del cliente
    void setEstado(const QString &nuevoEstado) { estado = nuevoEstado; }

    // Método para agregar un producto a la lista de compras
    void agregarProducto(const QString &producto, int cantidad) {
        if (listaCompra.contains(producto)) { // Si el producto ya está en la lista
            if (listaCompra[producto] + cantidad <= 3) { // Verifica que no se exceda el límite
                listaCompra[producto] += cantidad; // Aumenta la cantidad
            } else {
                listaCompra[producto] = 3; // Establece el máximo permitido
            }
        } else { // Si el producto no está en la lista
            if (cantidad <= 3) { // Verifica que no se exceda el límite
                listaCompra[producto] = cantidad; // Agrega el producto
            } else {
                listaCompra[producto] = 3; // Establece el máximo permitido
            }
        }
    }

    // Método para eliminar un producto de la lista de compras
    void eliminarProducto(const QString &producto) { listaCompra.remove(producto); }

    // Método para generar datos aleatorios del cliente
    void generarDatos() {
        nombre = QString::fromStdString(GenerarNombre()); // Genera un nombre aleatorio
        cedula = QString::fromStdString(GenerarCedula()); // Genera una cédula aleatoria
        telefono = QString::fromStdString(GenerarTelefono()); // Genera un teléfono aleatorio
        generarListaCompra(); // Genera una lista de compras aleatoria
    }

    // Método para generar una lista de compras aleatoria
    void generarListaCompra() {
        QStringList productosDisponibles = { // Lista de productos disponibles
                "Harina PA'NA",
                "Azucar fitnes Konfit",
                "Harina Robin 'No tan' Hood",
                "Espagueti Don Barilla",
                "Pan Arabe K-Bomb",
                "Pan Campesino",
                "Pan Frances",
                "Pan Pimbo",
                "Pasta de dientes Guin-date",
                "Jabon Dove",
                "Shampoo para calvos Pantene",
                "Shampoo para Hombres 27 en 1",
                "Con Fley",
                "Flops",
                "Chocolates Savoy",
                "Chupetas BomBum",
                "Carne de res",
                "Huevos",
                "Pollo",
                "Pescado",
                "Lechuga",
                "Repollo",
                "Zanahorias",
                "Leche",
                "Arroz",
                "Frijoles",
                "Aceite",
                "Papas",
                "Cebolla",
            };

        std::random_device rd; // Generador de números aleatorios
        std::mt19937 gen(rd()); // Motor de números aleatorios
        std::uniform_int_distribution<> disProductos(1, 15); // Rango para el número de productos
        std::uniform_int_distribution<> disCantidad(1, 3); // Rango para la cantidad de cada producto

        int totalProductos = 0; // Contador de productos totales
        int numProductos = disProductos(gen); // Número de productos a generar

        for (int i = 0; i < numProductos; ++i) {
            if (totalProductos >= 30) break; // Limita la lista a 30 productos

            // Selecciona un producto aleatorio
            QString producto = productosDisponibles[std::uniform_int_distribution<>(0, productosDisponibles.size() - 1)(gen)];
            int cantidad = disCantidad(gen); // Selecciona una cantidad aleatoria

            if (totalProductos + cantidad > 30) { // Ajusta la cantidad si se excede el límite
                cantidad = 30 - totalProductos;
            }

            agregarProducto(producto, cantidad); // Agrega el producto a la lista
            totalProductos += cantidad; // Actualiza el contador de productos
        }
    }

private:
    int id; // ID del cliente
    QString nombre; // Nombre del cliente
    QString estado; // Estado del cliente (ej: "Comprando")
    QMap<QString, int> listaCompra; // Lista de compras con cantidades
    QString cedula; /// Cédula del cliente
    QString telefono; // Teléfono del cliente
};

// Clase para gestionar una lista de clientes
class GestorClientes {
public:
    // Método para agregar un cliente
    void agregarCliente(const Cliente &cliente) {
        clientes[cliente.getId()] = cliente; // Almacena el cliente en el mapa
    }

    // Método para eliminar un cliente
    void eliminarCliente(int id) {
        clientes.remove(id); // Elimina el cliente del mapa
    }

    // Método para obtener un cliente por su ID
    Cliente obtenerCliente(int id) const {
        return clientes.value(id); // Retorna el cliente
    }

    // Método para obtener todos los clientes
    QList<Cliente> obtenerTodosLosClientes() const {
        return clientes.values(); // Retorna una lista de clientes
    }

    // Método para verificar si un cliente existe
    bool existeCliente(int id) const {
        return clientes.contains(id); // Retorna true si el cliente existe
    }

    // Método para obtener los productos vendidos
    QMap<QString, int> obtenerProductosVendidos() const {
        QMap<QString, int> productosVendidos; // Mapa para almacenar los productos vendidos

        // Recorre todos los clientes
        for (const Cliente &cliente : clientes.values()) {
            QMap<QString, int> listaCompra = cliente.getListaCompra(); // Obtiene la lista de compras del cliente
            for (auto it = listaCompra.begin(); it != listaCompra.end(); ++it) {
                productosVendidos[it.key()] += it.value(); // Suma las cantidades vendidas
            }
        }

        return productosVendidos; // Retorna el mapa de productos vendidos
    }

private:
    QMap<int, Cliente> clientes; // Mapa que almacena los clientes por su ID
};

// Clase para mostrar una ventana con los productos vendidos
class VentanaProductosVendidos : public QDialog {
public:
    VentanaProductosVendidos(const QMap<QString, int> &productosVendidos, QWidget *padre = nullptr)
        : QDialog(padre) {
        setWindowTitle("Productos Vendidos"); // Título de la ventana
        setFixedSize(500, 700); // Tamaño fijo de la ventana

        // Aplicar estilos CSS a la ventana
        setStyleSheet(R"(
            QDialog {
                background-color: #f0f0f0;
                font-family: "Arial";
                font-size: 14px;
            }
            QTableWidget {
                background-color: white;
                border: 1px solid #ccc;
                border-radius: 5px;
                gridline-color: #ddd;
            }
            QTableWidget::item {
                padding: 5px;
            }
            QHeaderView::section {
                background-color: #0078d7;
                color: white;
                padding: 5px;
                font-weight: bold;
                border: none;
            }
            QHeaderView::section:hover {
                background-color: #005bb5;
            }
            QTableWidget::item:selected {
                background-color: #0078d7;
                color: white;
            }
            QPushButton {
                background-color: #0078d7;
                color: white;
                padding: 5px 10px;
                border-radius: 3px;
                font-size: 14px;
            }
            QPushButton:hover {
                background-color: #005bb5;
            }
        )");

        // Crear una tabla para mostrar los productos vendidos
        QTableWidget *tablaProductos = new QTableWidget(this);
        tablaProductos->setColumnCount(2); // Dos columnas: Producto y Cantidad Vendida
        tablaProductos->setHorizontalHeaderLabels({"Producto", "Cantidad Vendida"}); // Encabezados de las columnas
        tablaProductos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Ajustar el ancho de las columnas
        tablaProductos->setEditTriggers(QAbstractItemView::NoEditTriggers); // Hacer la tabla no editable

        // Ordenar los productos de mayor a menor cantidad vendida
        QList<QPair<QString, int>> listaProductos;
        for (auto it = productosVendidos.begin(); it != productosVendidos.end(); ++it) {
            listaProductos.append(qMakePair(it.key(), it.value())); // Agrega cada producto a la lista
        }

        std::sort(listaProductos.begin(), listaProductos.end(), [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
            return a.second > b.second; // Ordena por cantidad vendida (de mayor a menor)
        });

        tablaProductos->setRowCount(listaProductos.size()); // Establece el número de filas
        int fila = 0;
        for (const auto &producto : listaProductos) {
            tablaProductos->setItem(fila, 0, new QTableWidgetItem(producto.first)); // Columna de producto
            tablaProductos->setItem(fila, 1, new QTableWidgetItem(QString::number(producto.second))); // Columna de cantidad
            fila++;
        }

        // Crear un layout vertical para la ventana
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(tablaProductos); // Agrega la tabla al layout
        setLayout(layout); // Establece el layout en la ventana
    }
};

// Clase para generar una factura
class Factura {
    public:
        Factura(const Cliente &cliente) : cliente(cliente) {} // Constructor que recibe un cliente

        // Método para generar la factura
        void generarFactura() {
            contenido.clear(); // Limpia el contenido anterior
            contenido += "================================================\n";
            contenido += "                SUPERMERCADO UNEG               \n";
            contenido += "================================================\n";
            contenido += "Cliente: " + cliente.getNombre() + "\n"; // Nombre del cliente
            contenido += "Cédula:  " + cliente.getCedula() + "\n"; // Cédula del cliente
            contenido += "Teléfono: " + cliente.getTelefono() + "\n"; // Teléfono del cliente
            contenido += "------------------------------------------------\n";
            contenido += "Productos                     Cant.   Subtotal  \n"; // Encabezado de la tabla
            contenido += "------------------------------------------------\n";

            double total = 0.0; // Total a pagar
            QMap<QString, int> listaCompra = cliente.getListaCompra(); // Obtiene la lista de compras del cliente
            for (auto it = listaCompra.begin(); it != listaCompra.end(); ++it) {
                QString producto = it.key(); // Nombre del producto
                int cantidad = it.value(); // Cantidad del producto
                double precio = obtenerPrecioProducto(producto); // Obtiene el precio del producto
                double subtotal = precio * cantidad; // Calcula el subtotal

                // Formatea las columnas para que estén alineadas
                contenido += QString("%1 %2 %3\n")
                                 .arg(producto, -30) // Columna de producto
                                 .arg(cantidad, 5) // Columna de cantidad
                                 .arg(subtotal, 10, 'f', 2); // Columna de subtotal
                total += subtotal; // Suma al total
            }

            contenido += "------------------------------------------------\n";
            contenido += "Total a pagar: $" + QString::number(total, 'f', 2) + "\n"; // Muestra el total
            contenido += "================================================\n";
        }

        // Método para obtener el contenido de la factura
        QString getContenido() const { return contenido; }

    private:
        Cliente cliente; // Cliente asociado a la factura
        QString contenido; // Contenido de la factura

        // Mapa de precios de los productos
        QMap<QString, double> precios = {
            {"Harina PA'NA", 2.30},
            {"Azucar fitnes Konfit", 1.75},
            {"Harina Robin 'No tan' Hood", 2.10},
            {"Espagueti Don Barilla", 3.20},
            {"Pan Arabe K-Bomb", 1.90},
            {"Pan Campesino", 1.50},
            {"Pan Frances", 1.80},
            {"Pan Pimbo", 1.25},
            {"Pasta de dientes Guin-date", 4.50},
            {"Jabon Dove", 2.75},
            {"Shampoo para calvos Pantene", 5.00},
            {"Shampoo para Hombres 27 en 1", 6.00},
            {"Con Fley", 0.50},
            {"Flops", 0.75},
            {"Chocolates Savoy", 2.25},
            {"Chupetas BomBum", 0.30},
            {"Carne de res", 7.00},
            {"Huevos", 1.20},
            {"Pollo", 5.00},
            {"Pescado", 6.50},
            {"Lechuga", 0.90},
            {"Repollo", 0.60},            
            {"Zanahorias", 0.80},
            {"Leche", 1.20},
            {"Arroz", 2.00},
            {"Frijoles", 1.80},
            {"Aceite", 3.50},
            {"Papas", 1.00},
            {"Cebolla", 0.50}
        };

        // Método para obtener el precio de un producto
        double obtenerPrecioProducto(const QString &producto) {
            return precios.value(producto, 0.0); // Retorna 0.0 si el producto no existe
        }
    };

// Clase para mostrar un diálogo con los detalles de un cliente
class DialogoCliente : public QDialog {
    public:
        DialogoCliente(const Cliente &cliente, QWidget *padre = nullptr)
            : QDialog(padre) {
            // Configuración básica de la ventana
            setWindowTitle("Detalles del Cliente"); // Título de la ventana
            setWindowIcon(QIcon(":/iconos/cliente.png")); // Ícono de la ventana
            setFixedSize(500, 600); // Tamaño fijo de la ventana
    
            // Estilo global usando CSS
            setStyleSheet(
                "QDialog { background-color: #f0f0f0; }"
                "QLabel { font-size: 14px; color: #333; }"
                "QGroupBox { font-size: 16px; font-weight: bold; border: 2px solid #ccc; border-radius: 5px; margin-top: 10px; }"
                "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 3px; }"
                "QTableWidget { background-color: white; border: 1px solid #ccc; font-size: 14px; }"
                "QHeaderView::section { background-color: #0078d7; color: white; padding: 5px; }"
            );
    
            // Grupo para la información del cliente
            QGroupBox *grupoCliente = new QGroupBox("Información del Cliente", this);
    
            QLabel *labelId = new QLabel("ID: " + QString::number(cliente.getId()), this); // Muestra el ID del cliente
            QLabel *labelNombre = new QLabel("Nombre: " + cliente.getNombre(), this); // Muestra el nombre del cliente
            QLabel *labelEstado = new QLabel("Estado: " + cliente.getEstado(), this); // Muestra el estado del cliente
            QLabel *labelCedula = new QLabel("Cédula: " + cliente.getCedula(), this); // Muestra la cédula del cliente
            QLabel *labelTelefono = new QLabel("Teléfono: " + cliente.getTelefono(), this); // Muestra el teléfono del cliente
    
            // Layout para el grupo de información del cliente
            QVBoxLayout *layoutCliente = new QVBoxLayout;
            layoutCliente->addWidget(labelId);
            layoutCliente->addWidget(labelNombre);
            layoutCliente->addWidget(labelEstado);
            layoutCliente->addWidget(labelCedula);
            layoutCliente->addWidget(labelTelefono);
            grupoCliente->setLayout(layoutCliente);
    
            // Tabla para la lista de la compra
            QTableWidget *tablaListaCompra = new QTableWidget(this);
            tablaListaCompra->setColumnCount(2); // Dos columnas: Producto y Cantidad
            tablaListaCompra->setHorizontalHeaderLabels({"Producto", "Cantidad"}); // Encabezados de las columnas
            tablaListaCompra->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Ajustar el ancho de las columnas
            tablaListaCompra->setEditTriggers(QAbstractItemView::NoEditTriggers); // Hacer la tabla no editable

            // Llenar la tabla con la lista de la compra
            QMap<QString, int> listaCompra = cliente.getListaCompra(); // Obtiene la lista de compras del cliente
            tablaListaCompra->setRowCount(listaCompra.size()); // Establece el número de filas
            int fila = 0;
            for (auto it = listaCompra.begin(); it != listaCompra.end(); ++it) {
                tablaListaCompra->setItem(fila, 0, new QTableWidgetItem(it.key())); // Columna de producto
                tablaListaCompra->setItem(fila, 1, new QTableWidgetItem(QString::number(it.value()))); // Columna de cantidad
                fila++;
            }
    
            // Layout principal de la ventana
            QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
            layoutPrincipal->addWidget(grupoCliente); // Agrega el grupo de información del cliente
            layoutPrincipal->addWidget(new QLabel("Lista de la Compra:", this)); // Etiqueta para la lista de compras
            layoutPrincipal->addWidget(tablaListaCompra); // Agrega la tabla de la lista de compras

            setLayout(layoutPrincipal); // Establece el layout en la ventana
        }
    };

// Clase para mostrar una ventana con la factura
class VentanaFactura : public QDialog {
public:
    VentanaFactura(const QString &rutaArchivo, QWidget *padre = nullptr) : QDialog(padre) {
        setWindowTitle("Detalles de la Factura"); // Título de la ventana
        setFixedSize(420, 600); // Tamaño fijo de la ventana

        QTextEdit *editorTexto = new QTextEdit(this); // Editor de texto para mostrar la factura
        editorTexto->setReadOnly(true); // Hacer el editor de texto no editable
        QFont font("Courier New"); // O "Monospace", "Consolas", "Lucida Console"
        font.setPointSize(10); // Ajusta el tamaño de la fuente si es necesario
        font.setWeight(QFont::Bold); // Pone el texto en negritas
        editorTexto->setFont(font);
        editorTexto->setWordWrapMode(QTextOption::NoWrap); // Desactiva el ajuste de línea
        editorTexto->setTextColor(Qt::black); // Establece el color del texto a negro

        QFile archivo(rutaArchivo); // Abre el archivo de la factura
        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) { // Si el archivo se abre correctamente
            QTextStream in(&archivo); // Lee el contenido del archivo
            editorTexto->setText(in.readAll()); // Muestra el contenido en el editor de texto
            archivo.close(); // Cierra el archivo
        } else {
            editorTexto->setText("Error: No se pudo abrir el archivo."); // Muestra un mensaje de error
        }

        QVBoxLayout *layout = new QVBoxLayout(this); // Layout vertical para la ventana
        layout->addWidget(editorTexto); // Agrega el editor de texto al layout
        setLayout(layout); // Establece el layout en la ventana
    }
};

// Clase que representa un carrito móvil en la escena
class CarritoMovil : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    CarritoMovil(const QPixmap &pixmap, Cliente *cliente, QGraphicsItem *padre = nullptr)
        : QObject(nullptr),
        QGraphicsPixmapItem(pixmap, padre),
        indiceActual(0),
        velocidad(1),
        cliente(cliente),
        estaPausado(false) {
        textoId = new QGraphicsTextItem(QString::number(cliente->getId()), this); // Muestra el ID del cliente en el carrito
        textoId->setDefaultTextColor(Qt::black); // Color del texto
        textoId->setFont(QFont("Arial", 12)); // Fuente del texto

        QRectF rectCarrito = boundingRect(); // Obtiene el rectángulo del carrito
        textoId->setPos(rectCarrito.width() / 2 - textoId->boundingRect().width() / 2, -20); // Posiciona el texto

        temporizadorPausa = new QTimer(this); // Temporizador para pausar el movimiento
        connect(temporizadorPausa, &QTimer::timeout, this, &CarritoMovil::reanudarMovimiento); // Conecta el temporizador al método de reanudación
    }

    void establecerRuta(const QList<QPointF>& nuevaRuta, const QList<QPointF>& puntosDetencion, const QList<QPointF>& posicionesCriticas) {
        ruta = nuevaRuta; // Establece la ruta del carrito
        puntosDeDetencion = puntosDetencion; // Establece los puntos de detención
        this->posicionesCriticas = posicionesCriticas; // Establece las posiciones críticas
    }

    void advance(int fase) override {
        if (fase && !ruta.isEmpty() && !estaPausado) { // Si el carrito está en movimiento
            QPointF objetivo = ruta[indiceActual]; // Obtiene el siguiente punto de la ruta
            QPointF posicionActual = pos(); // Obtiene la posición actual del carrito

            QPointF direccion = objetivo - posicionActual; // Calcula la dirección hacia el objetivo
            qreal distancia = QLineF(posicionActual, objetivo).length(); // Calcula la distancia al objetivo

            if (distancia > 1) { // Si la distancia es mayor a 1
                direccion /= distancia; // Normaliza la dirección
                QPointF nuevaPosicion = posicionActual + direccion * velocidad; // Calcula la nueva posición

                // Verifica si la nueva posición es una posición crítica y está ocupada
                bool posicionCriticaOcupada = false;
                if (posicionesCriticas.contains(objetivo)) {
                    for (QGraphicsItem *item : scene()->items()) {
                        if (item != this && item->type() == QGraphicsPixmapItem::Type) {
                            if (QLineF(objetivo, item->pos()).length() < 10) { // Tolerancia de 10 píxeles
                                posicionCriticaOcupada = true;
                                break;
                            }
                        }
                    }
                }

                if (!posicionCriticaOcupada) {
                    setPos(nuevaPosicion); // Mueve el carrito a la nueva posición
                } else {
                    pausarMovimiento(); // Pausa el movimiento si la posición crítica está ocupada
                }
            } else {
                // Verifica si el punto actual es un punto de detención
                if (puntosDeDetencion.contains(objetivo)) {
                    pausarMovimiento(); // Detiene el carrito
                } else {
                    indiceActual++; // Continúa al siguiente punto
                }

                if (indiceActual >= ruta.size()) { // Si el carrito llegó al final de la ruta
                    emit carritoCompletado(cliente->getId(), "../datos/facturas/factura_ultimo_cliente.txt"); // Emite la señal de finalización
                    if (scene()) scene()->removeItem(this); // Elimina el carrito de la escena
                    deleteLater(); // Libera la memoria del carrito
                }
            }
        }
    }

    void pausarMovimiento() {
        estaPausado = true; // Pausa el movimiento

        // Genera un tiempo de pausa aleatorio entre 7 y 10 segundos
        int duracionPausa = QRandomGenerator::global()->bounded(7000, 10000); // Entre 7000 ms y 10000 ms
        temporizadorPausa->start(duracionPausa); // Inicia el temporizador de pausa
    }

    void reanudarMovimiento() {
        estaPausado = false; // Reanuda el movimiento
        temporizadorPausa->stop(); // Detiene el temporizador de pausa
        indiceActual++; // Avanza al siguiente punto
        if (indiceActual < ruta.size()) {
            advance(1); // Avanza al siguiente punto
        } else {
            emit carritoCompletado(cliente->getId(), "../datos/facturas/factura_ultimo_cliente.txt"); // Emite la señal de finalización
            if (scene()) scene()->removeItem(this); // Elimina el carrito de la escena
            deleteLater(); // Libera la memoria del carrito
        }
    }

signals:
    void carritoCompletado(int clienteId, const QString &rutaArchivo); // Señal que se emite cuando el carrito completa su ruta

private:
    QList<QPointF> ruta; // Ruta que sigue el carrito
    QList<QPointF> puntosDeDetencion; // Puntos donde el carrito se detiene
    QList<QPointF> posicionesCriticas; // Posiciones críticas donde el carrito puede colisionar
    int indiceActual; // Índice del punto actual en la ruta
    qreal velocidad; // Velocidad del carrito
    Cliente *cliente; // Cliente asociado al carrito
    QGraphicsTextItem *textoId; // Texto que muestra el ID del cliente
    QTimer *temporizadorPausa; // Temporizador para pausar el movimiento
    bool estaPausado; // Indica si el carrito está pausado
};

// Clase para generar carritos en la escena
class GeneradorCarritos : public QObject {
    Q_OBJECT

public:
    GeneradorCarritos(QGraphicsScene *escena, GestorClientes *gestorClientes, QHBoxLayout *layoutClientes, QTextEdit *textEditFactura, QObject *padre = nullptr)
        : QObject(padre), escena(escena), gestorClientes(gestorClientes), layoutClientes(layoutClientes), textEditFactura(textEditFactura), generando(false), carritosActivos(0) {
        ruta = { // Ruta que siguen los carritos
            QPointF(0, 100),
            QPointF(50, 100),
            QPointF(50, 400),
            QPointF(50, 230),
            QPointF(265, 230),
            QPointF(265, 100),
            QPointF(265, 400),
            QPointF(265, 230),
            QPointF(475, 230),
            QPointF(475, 100),
            QPointF(475, 400),
            QPointF(475, 230),
            QPointF(700, 230),
            QPointF(700, 270),
            QPointF(850, 270),
        };

        puntosDeDetencion = { // Puntos donde los carritos se detienen
            QPointF(50, 100),
            QPointF(50, 400),
            QPointF(265, 100), // Punto de detención 2
            QPointF(265, 400),
            QPointF(475, 100),
            QPointF(475, 400),
            QPointF(850, 270),
        };

        temporizador = new QTimer(this); // Temporizador para generar carritos
        connect(temporizador, &QTimer::timeout, this, &GeneradorCarritos::generarCarrito); // Conecta el temporizador al método de generación
    }

    void iniciarGeneracion() {
        if (!generando) {
            temporizador->start(15000); // Genera un carrito cada 15 segundos
            generando = true;
        }
    }

    void detenerGeneracion() {
        if (generando) {
            temporizador->stop(); // Detiene la generación de carritos
            generando = false;
        }
    }

private slots:
    void generarCarrito() {
        if (carritosActivos >= 3) { // Si ya hay 3 carritos activos
            qDebug() << "Límite de carritos alcanzado. No se puede generar más.";
            return; // No genera más carritos
        }

        QPixmap carritoPixmap("/home/abraham/supermercado_proyecto/carrito.png"); // Carga la imagen del carrito
        if (carritoPixmap.isNull()) {
            qDebug() << "Error: No se pudo cargar la imagen del carrito.";
            return;
        }

        QPixmap carritoEscalado = carritoPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Escala la imagen del carrito

        int nuevoId = gestorClientes->obtenerTodosLosClientes().isEmpty() ? 1 : gestorClientes->obtenerTodosLosClientes().last().getId() + 1; // Genera un ID único
        Cliente cliente(nuevoId, "", "Comprando"); // Crea un nuevo cliente
        cliente.generarDatos(); // Genera datos aleatorios para el cliente

        gestorClientes->agregarCliente(cliente); // Agrega el cliente al gestor

        actualizarInformacionClientes(layoutClientes, gestorClientes->obtenerTodosLosClientes()); // Actualiza la información de los clientes

        CarritoMovil *carrito = new CarritoMovil(carritoEscalado, new Cliente(cliente)); // Crea un nuevo carrito
        escena->addItem(carrito); // Agrega el carrito a la escena
        carrito->establecerRuta(ruta, puntosDeDetencion, posicionesCriticas); // Establece la ruta del carrito

        carritosActivos++; // Incrementa el contador de carritos activos

        connect(carrito, &CarritoMovil::carritoCompletado, this, &GeneradorCarritos::manejarCarritoCompletado); // Conecta la señal de finalización del carrito
    }

    void manejarCarritoCompletado(int clienteId, const QString &rutaArchivo) {
        // Genera la factura del cliente
        Cliente cliente = gestorClientes->obtenerCliente(clienteId);
        Factura factura(cliente);
        factura.generarFactura();

        // Guarda la factura del último cliente en un archivo
        QFile archivoFacturaUltimo("../datos/facturas/factura_ultimo_cliente.txt");
        if (archivoFacturaUltimo.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&archivoFacturaUltimo);
            out << factura.getContenido(); // Escribe la factura en el archivo
            archivoFacturaUltimo.close();
        } else {
            qDebug() << "Error: No se pudo abrir el archivo para escribir la factura del último cliente.";
        }

        // Muestra la factura en el QTextEdit
        textEditFactura->setText(factura.getContenido());

        // Maneja la lista de las últimas 10 facturas
        invoices.append(factura.getContenido()); // Agrega la factura a la lista
        if (invoices.size() > 10) {
            invoices.removeFirst(); // Elimina la factura más antigua
        }

        // Escribe todas las facturas en un archivo
        QFile archivoFacturas("../datos/facturas/facturas.txt");
        if (archivoFacturas.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream allInvoicesStream(&archivoFacturas);
            for (const QString &invoice : invoices) {
                allInvoicesStream << invoice << "\n"; // Escribe cada factura en el archivo
            }
            archivoFacturas.close();
        } else {
            qDebug() << "Error: No se pudo abrir el archivo para escribir todas las facturas.";
        }

        // Elimina el cliente del gestor
        gestorClientes->eliminarCliente(clienteId);
        actualizarInformacionClientes(layoutClientes, gestorClientes->obtenerTodosLosClientes()); // Actualiza la información de los clientes

        carritosActivos--; // Reduce el contador de carritos activos
    }

private:
    QGraphicsScene *escena; // Escena donde se mueven los carritos
    QList<QPointF> ruta; // Ruta que siguen los carritos
    QList<QPointF> puntosDeDetencion; // Puntos donde los carritos se detienen
    QList<QPointF> posicionesCriticas; // Posiciones críticas donde los carritos pueden colisionar
    QTimer *temporizador; // Temporizador para generar carritos
    bool generando; // Indica si se están generando carritos
    GestorClientes *gestorClientes; // Gestor de clientes
    QHBoxLayout *layoutClientes; // Layout para mostrar la información de los clientes
    QTextEdit *textEditFactura; // Editor de texto para mostrar la factura
    QList<QString> invoices; // Lista de las últimas 10 facturas
    int carritosActivos; // Contador de carritos activos

    // Método para actualizar la información de los clientes en la interfaz
    void actualizarInformacionClientes(QHBoxLayout *layoutClientes, const QList<Cliente> &clientes) {
        QLayoutItem *item;
        while ((item = layoutClientes->takeAt(0)) != nullptr) { // Limpia el layout actual
            delete item->widget();
            delete item;
        }

        for (const Cliente &cliente : clientes) { // Recorre todos los clientes
            QWidget *widgetCliente = new QWidget; // Crea un widget para el cliente
            QVBoxLayout *layoutCliente = new QVBoxLayout(widgetCliente); // Layout vertical para el widget

            QString texto = QString("ID: %1\nNombre: %2\nEstado: %3")
                                .arg(cliente.getId()) // ID del cliente
                                .arg(cliente.getNombre()) // Nombre del cliente
                                .arg(cliente.getEstado()); // Estado del cliente

            QLabel *labelCliente = new QLabel(texto); // Etiqueta con la información del cliente
            layoutCliente->addWidget(labelCliente); // Agrega la etiqueta al layout

            QPushButton *botonDetalles = new QPushButton("Ver detalles"); // Botón para ver detalles del cliente
            QObject::connect(botonDetalles, &QPushButton::clicked, [cliente]() {
                DialogoCliente *dialogo = new DialogoCliente(cliente); // Crea un diálogo con los detalles del cliente
                dialogo->exec(); // Muestra el diálogo
            });
            layoutCliente->addWidget(botonDetalles); // Agrega el botón al layout

            layoutClientes->addWidget(widgetCliente); // Agrega el widget al layout principal
        }
    }
};

// Función para cargar un archivo de texto en un QTextEdit
void cargarArchivoTxt(QTextEdit *textEdit, const QString &rutaArchivo) {
    QFile archivo(rutaArchivo); // Abre el archivo
    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) { // Si el archivo se abre correctamente
        QTextStream in(&archivo); // Lee el contenido del archivo
        textEdit->setText(in.readAll()); // Muestra el contenido en el QTextEdit
        archivo.close(); // Cierra el archivo
    } else {
        textEdit->setText("Error: No se pudo abrir el archivo."); // Muestra un mensaje de error
    }
}

// Función principal del programa
int main(int argc, char *argv[]) {
    QApplication aplicacion(argc, argv); // Crea la aplicación Qt

    QGraphicsScene escena; // Crea la escena gráfica
    escena.setSceneRect(0, 0, 1000, 600); // Establece el tamaño de la escena

    GestorClientes gestorClientes; // Crea el gestor de clientes

    QWidget *widgetClientes = new QWidget; // Widget para mostrar la información de los clientes
    QHBoxLayout *layoutClientes = new QHBoxLayout(widgetClientes); // Layout horizontal para los clientes
    widgetClientes->setMinimumWidth(400); // Establece el ancho mínimo del widget

    QTextEdit *textEditFactura = new QTextEdit; // Editor de texto para mostrar la factura
    textEditFactura->setReadOnly(true); // Hace el editor de texto no editable
    QFont fontFactura("Courier New");
    fontFactura.setPointSize(10);
    fontFactura.setWeight(QFont::Bold); // Pone el texto en negritas
    textEditFactura->setFont(fontFactura);
    textEditFactura->setWordWrapMode(QTextOption::NoWrap); // Desactiva el ajuste de línea
    textEditFactura->setTextColor(Qt::black); // Establece el color del texto a negro

    QSplitter *splitter = new QSplitter(Qt::Vertical); // Splitter para dividir la ventana
    splitter->addWidget(widgetClientes); // Agrega el widget de clientes al splitter

    QWidget *widgetArchivo = new QWidget; // Widget para mostrar el archivo de facturas
    QVBoxLayout *layoutArchivo = new QVBoxLayout(widgetArchivo); // Layout vertical para el widget
    layoutArchivo->addWidget(textEditFactura); // Agrega el editor de texto al layout
    splitter->addWidget(widgetArchivo); // Agrega el widget al splitter

    GeneradorCarritos generador(&escena, &gestorClientes, layoutClientes, textEditFactura); // Crea el generador de carritos

    QPixmap imagenFondo("/home/abraham/supermercado_proyecto/fondo.png"); // Carga la imagen de fondo
    if (imagenFondo.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen de fondo.";
        return -1;
    }

    QPixmap fondoEscalado = imagenFondo.scaled(306, 204, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Escala la imagen de fondo
    QBrush fondo(fondoEscalado); // Crea un pincel con la imagen de fondo
    fondo.setStyle(Qt::TexturePattern); // Establece el estilo del pincel
    escena.setBackgroundBrush(fondo); // Establece el fondo de la escena

    QPixmap imagenEstante1("/home/abraham/supermercado_proyecto/estante1.png"); // Carga la imagen del estante 1
    QPixmap imagenEstante2("/home/abraham/supermercado_proyecto/estante2.png"); // Carga la imagen del estante 2
    QPixmap imagenEstante3("/home/abraham/supermercado_proyecto/estante3.png"); // Carga la imagen del estante 3
    QPixmap imagenCaja("../img/caja.png"); // Carga la imagen de la caja

    if (imagenEstante1.isNull() || imagenCaja.isNull() || imagenEstante2.isNull() || imagenEstante3.isNull()) {
        qDebug() << "Error: No se pudieron cargar las imágenes de los elementos.";
        return -1;
    }

    QPixmap estante1Escalado = imagenEstante1.scaled(203, 272, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Escala la imagen del estante 1
    QPixmap estante2Escalado = imagenEstante2.scaled(181, 244, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Escala la imagen del estante 2
    QPixmap estante3Escalado = imagenEstante3.scaled(181, 244, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Escala la imagen del estante 3
    QPixmap cajaEscalado = imagenCaja.scaled(300, 217, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Escala la imagen de la caja

    QGraphicsPixmapItem *estanteItem1 = new QGraphicsPixmapItem(estante1Escalado); // Crea un ítem para el estante 1
    QGraphicsPixmapItem *estanteItem2 = new QGraphicsPixmapItem(estante2Escalado); // Crea un ítem para el estante 2
    QGraphicsPixmapItem *estanteItem3 = new QGraphicsPixmapItem(estante3Escalado); // Crea un ítem para el estante 3
    QGraphicsPixmapItem *estanteItem4 = new QGraphicsPixmapItem(estante1Escalado); // Crea un ítem para el estante 4
    QGraphicsPixmapItem *estanteItem5 = new QGraphicsPixmapItem(estante2Escalado); // Crea un ítem para el estante 5
    QGraphicsPixmapItem *estanteItem6 = new QGraphicsPixmapItem(estante3Escalado); // Crea un ítem para el estante 6
    QGraphicsPixmapItem *cajaItem = new QGraphicsPixmapItem(cajaEscalado); // Crea un ítem para la caja

    // Posiciona los estantes y la caja en la escena
    estanteItem1->setPos(100, 10);
    estanteItem2->setPos(365, 13);
    estanteItem3->setPos(570, 13);
    estanteItem4->setPos(100, 300);
    estanteItem5->setPos(365, 303);
    estanteItem6->setPos(570, 303);
    cajaItem->setPos(800, 160);

    // Agrega los estantes y la caja a la escena
    escena.addItem(estanteItem1);
    escena.addItem(estanteItem2);
    escena.addItem(estanteItem3);
    escena.addItem(estanteItem4);
    escena.addItem(estanteItem5);
    escena.addItem(estanteItem6);
    escena.addItem(cajaItem);

    // Configura la vista de la escena
    QGraphicsView vista(&escena); // Crea la vista de la escena
    vista.setWindowTitle("Generador de Carritos en Qt"); // Título de la ventana
    vista.resize(1000, 600); // Tamaño de la ventana

    // Botones para iniciar la generación de carritos y mostrar facturas
    QPushButton *botonGeneracion = new QPushButton("Iniciar Generación"); // Botón para iniciar la generación de carritos
    botonGeneracion->setCheckable(true); // Hace el botón checkable
    QPushButton *botonFacturas = new QPushButton("Mostrar Facturas"); // Botón para mostrar las facturas
    QPushButton *botonProductos = new QPushButton("Productos Vendidos"); // Botón para mostrar los productos vendidos

    // Layout para los botones
    QHBoxLayout *layoutBotones = new QHBoxLayout;
    layoutBotones->addWidget(botonGeneracion); // Agrega el botón de generación
    layoutBotones->addWidget(botonFacturas); // Agrega el botón de facturas
    layoutBotones->addWidget(botonProductos); // Agrega el botón de productos vendidos

    // Layout principal
    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(layoutBotones); // Agrega el layout de botones
    layoutPrincipal->addWidget(&vista); // Agrega la vista de la escena

    // Layout final que incluye el QSplitter
    QHBoxLayout *layoutFinal = new QHBoxLayout;
    layoutFinal->addLayout(layoutPrincipal); // Agrega el layout principal
    layoutFinal->addWidget(splitter); // Agrega el splitter

    layoutFinal->setStretch(0, 3); // Establece el tamaño relativo del layout principal
    layoutFinal->setStretch(1, 1); // Establece el tamaño relativo del splitter

    // Configura la ventana principal
    QWidget ventana;
    ventana.setLayout(layoutFinal); // Establece el layout final en la ventana
    ventana.setFixedSize(1400, 700); // Tamaño fijo de la ventana
    ventana.show(); // Muestra la ventana

    // Conecta señales y slots
    QObject::connect(botonGeneracion, &QPushButton::toggled, [&](bool checked) {
        if (checked) {
            generador.iniciarGeneracion(); // Inicia la generación de carritos
            botonGeneracion->setText("Detener Generación"); // Cambia el texto del botón
        } else {
            generador.detenerGeneracion(); // Detiene la generación de carritos
            botonGeneracion->setText("Iniciar Generación"); // Cambia el texto del botón
        }
    });

    QObject::connect(botonFacturas, &QPushButton::clicked, [&]() {
        VentanaFactura *ventanaFactura = new VentanaFactura("../datos/facturas/facturas.txt"); // Crea una ventana de facturas
        ventanaFactura->exec(); // Muestra la ventana de facturas
    });

    QObject::connect(botonProductos, &QPushButton::clicked, [&]() {
        QMap<QString, int> productosVendidos = gestorClientes.obtenerProductosVendidos(); // Obtiene los productos vendidos
        VentanaProductosVendidos *dialogo = new VentanaProductosVendidos(productosVendidos); // Crea una ventana de productos vendidos
        dialogo->exec(); // Muestra la ventana
    });

    // Temporizador para avanzar la escena
    QTimer temporizador;
    QObject::connect(&temporizador, &QTimer::timeout, &escena, &QGraphicsScene::advance); // Conecta el temporizador al avance de la escena
    temporizador.start(10); // Inicia el temporizador

    return aplicacion.exec(); // Ejecuta la aplicación
}

#include "main.moc" // Incluye el archivo MOC generado por Qt