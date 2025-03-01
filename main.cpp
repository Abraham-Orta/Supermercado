/*#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QList>
#include <QPointF>
#include <QLineF>
#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextEdit>
#include <QStringList>
#include <algorithm>
#include <random>
#include <QMap>
#include <QSplitter>
#include <QCoreApplication>
#include "xlsxdocument.h"
#include <QScopedPointer>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QString>
#include <ctime>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <QVector>

// Declaración de variables globales
std::vector<std::string> cedulasgeneradas;
std::vector<std::string> telefonogeneradas;


std::string GenerarNombre() {
    // Lista de nombres y apellidos (puedes cargarlos desde un archivo Excel si lo prefieres)
    std::vector<std::string> nombres = {"Juan", "María", "Carlos", "Ana", "Luis"};
    std::vector<std::string> apellidos = {"Pérez", "Gómez", "López", "Martínez", "Rodríguez","Rivas"};

    // Generar índices aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribNombre(0, nombres.size() - 1);
    std::uniform_int_distribution<> distribApellido(0, apellidos.size() - 1);

    // Seleccionar un nombre y un apellido aleatorio
    std::string nombreCompleto = nombres[distribNombre(gen)] + " " + apellidos[distribApellido(gen)];
    return nombreCompleto;
}


std::string GenerarCedula() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(4000000, 40000000);

    std::string cedula;
    bool Duplicada;

    do {
        int cedulaNum = distrib(gen);
        cedula = std::to_string(cedulaNum);
        Duplicada = (std::find(cedulasgeneradas.begin(), cedulasgeneradas.end(), cedula) != cedulasgeneradas.end());
    } while (Duplicada);

    cedulasgeneradas.push_back(cedula);
    return cedulasgeneradas.back();
}

std::string GenerarTelefono() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib1(500000, 40000000);
    std::uniform_int_distribution<int> distrib2(1, 5);

    int prefijo;
    std::string telefono;
    bool Duplicada;

    do {
        int telefonoNum = distrib1(gen);
        prefijo = distrib2(gen);
        std::stringstream completo;

        switch (prefijo) {
        case 1:
            completo << "0424" << telefonoNum;
            break;
        case 2:
            completo << "0414" << telefonoNum;
            break;
        case 3:
            completo << "0412" << telefonoNum;
            break;
        case 4:
            completo << "0416" << telefonoNum;
            break;
        case 5:
            completo << "0426" << telefonoNum;
            break;
        }

        telefono = completo.str();
        Duplicada = (std::find(telefonogeneradas.begin(), telefonogeneradas.end(), telefono) != telefonogeneradas.end());
    } while (Duplicada);

    telefonogeneradas.push_back(telefono);
    return telefonogeneradas.back();
}

class Cliente {
public:
    Cliente() : id(0), nombre(""), estado("") {}
    Cliente(int id, const QString &nombre, const QString &estado, const QMap<QString, int> &listaCompra = QMap<QString, int>())
        : id(id), nombre(nombre), estado(estado), listaCompra(listaCompra) {}

    int getId() const { return id; }
    QString getNombre() const { return nombre; }
    QString getEstado() const { return estado; }
    QMap<QString, int> getListaCompra() const { return listaCompra; }
    QString getCedula() const { return cedula; }
    QString getTelefono() const { return telefono; }

    void setEstado(const QString &nuevoEstado) { estado = nuevoEstado; }

    // Método para agregar un producto con límites
    void agregarProducto(const QString &producto, int cantidad) {
        if (listaCompra.contains(producto)) {
            // Verificar que no se exceda el límite de 3 por producto
            if (listaCompra[producto] + cantidad <= 3) {
                listaCompra[producto] += cantidad;
            } else {
                listaCompra[producto] = 3; // Establecer el máximo permitido
            }
        } else {
            // Verificar que no se exceda el límite de 3 por producto
            if (cantidad <= 3) {
                listaCompra[producto] = cantidad;
            } else {
                listaCompra[producto] = 3; // Establecer el máximo permitido
            }
        }
    }

    void eliminarProducto(const QString &producto) { listaCompra.remove(producto); }

    void generarDatos() {
        nombre = QString::fromStdString(GenerarNombre());
        cedula = QString::fromStdString(GenerarCedula());
        telefono = QString::fromStdString(GenerarTelefono());
        generarListaCompra();
    }

    // Método para generar la lista de compra con límites
    void generarListaCompra() {
        QStringList productosDisponibles = {"Manzanas", "Pan", "Leche", "Arroz", "Frijoles", "Aceite", "Pollo", "Papas", "Cebolla"};

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> disProductos(1, 5); // Número aleatorio de productos (1 a 5)
        std::uniform_int_distribution<> disCantidad(1, 3); // Cantidad aleatoria por producto (1 a 3)

        int totalProductos = 0; // Contador para la suma total de productos
        int numProductos = disProductos(gen);

        // Seleccionar productos aleatorios con cantidades
        for (int i = 0; i < numProductos; ++i) {
            if (totalProductos >= 30) break; // No superar el límite de 30 productos

            QString producto = productosDisponibles[std::uniform_int_distribution<>(0, productosDisponibles.size() - 1)(gen)];
            int cantidad = disCantidad(gen);

            // Verificar que no se exceda el límite de 30 productos en total
            if (totalProductos + cantidad > 30) {
                cantidad = 30 - totalProductos; // Ajustar la cantidad para no superar el límite
            }

            // Agregar el producto con la cantidad ajustada
            agregarProducto(producto, cantidad);
            totalProductos += cantidad;
        }
    }

private:
    int id;
    QString nombre;
    QString estado;
    QMap<QString, int> listaCompra; // Lista de compra con cantidades
    QString cedula;
    QString telefono;
};


// Clase para gestionar múltiples clientes
class GestorClientes {
public:
    void agregarCliente(const Cliente &cliente) {
        clientes[cliente.getId()] = cliente;
    }

    void eliminarCliente(int id) {
        clientes.remove(id);
    }

    Cliente obtenerCliente(int id) const {
        return clientes.value(id);
    }

    QList<Cliente> obtenerTodosLosClientes() const {
        return clientes.values();
    }

    bool existeCliente(int id) const {
        return clientes.contains(id);
    }

private:
    QMap<int, Cliente> clientes;
};

class Factura {
public:
    Factura(const Cliente &cliente) : cliente(cliente) {}

    void generarFactura() {
        // Crear el contenido de la factura
        contenido = "Factura para: " + cliente.getNombre() + "\n";
        contenido += "Cédula: " + cliente.getCedula() + "\n";
        contenido += "Teléfono: " + cliente.getTelefono() + "\n\n";
        contenido += "Productos:\n";

        double total = 0.0;
        QMap<QString, int> listaCompra = cliente.getListaCompra();
        for (auto it = listaCompra.begin(); it != listaCompra.end(); ++it) {
            double precio = obtenerPrecioProducto(it.key()); // Obtener el precio del producto
            double subtotal = precio * it.value();
            contenido += it.key() + " x " + QString::number(it.value()) + " = $" + QString::number(subtotal, 'f', 2) + "\n";
            total += subtotal;
        }

        contenido += "\nTotal: $" + QString::number(total, 'f', 2) + "\n";
    }

    void guardarFactura(const QString &rutaArchivo) {
        QFile archivo(rutaArchivo);
        if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&archivo);
            out << contenido;
            archivo.close();
        } else {
            qDebug() << "Error: No se pudo guardar la factura.";
        }
    }

    QString getContenido() const { return contenido; }

private:
    Cliente cliente;
    QString contenido;

    // Método para obtener el precio de un producto (puedes cargarlo desde una base de datos o archivo)
    double obtenerPrecioProducto(const QString &producto) {
        QMap<QString, double> precios = {
            {"Manzanas", 1.50},
            {"Pan", 0.80},
            {"Leche", 1.20},
            {"Arroz", 2.00},
            {"Frijoles", 1.80},
            {"Aceite", 3.50},
            {"Pollo", 5.00},
            {"Papas", 1.00},
            {"Cebolla", 0.50}
        };
        return precios.value(producto, 0.0); // Retorna 0.0 si el producto no está en la lista
    }
};

// Clase para el cuadro de diálogo de detalles del cliente
class DialogoCliente : public QDialog {
public:
    DialogoCliente(const Cliente &cliente, QWidget *padre = nullptr)
        : QDialog(padre) {
        setWindowTitle("Detalles del Cliente");
        setFixedSize(400, 300);

        QLabel *labelId = new QLabel("ID: " + QString::number(cliente.getId()), this);
        QLabel *labelNombre = new QLabel("Nombre: " + cliente.getNombre(), this);
        QLabel *labelEstado = new QLabel("Estado: " + cliente.getEstado(), this);
        QLabel *labelCedula = new QLabel("Cédula: " + cliente.getCedula(), this);
        QLabel *labelTelefono = new QLabel("Teléfono: " + cliente.getTelefono(), this);

        QTextEdit *textEditListaCompra = new QTextEdit(this);
        textEditListaCompra->setReadOnly(true);

        QString listaCompraTexto = "Lista de la compra:\n";
        QMap<QString, int> listaCompra = cliente.getListaCompra();
        for (auto it = listaCompra.begin(); it != listaCompra.end(); ++it) {
            listaCompraTexto += it.key() + ": " + QString::number(it.value()) + "\n";
        }

        textEditListaCompra->setText(listaCompraTexto);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(labelId);
        layout->addWidget(labelNombre);
        layout->addWidget(labelEstado);
        layout->addWidget(labelCedula);
        layout->addWidget(labelTelefono);
        layout->addWidget(textEditListaCompra);

        setLayout(layout);
    }
};

// Clase para la ventana de factura
class VentanaFactura : public QDialog {
public:
    VentanaFactura(const QString &rutaArchivo, QWidget *padre = nullptr) : QDialog(padre) {
        setWindowTitle("Detalles de la Factura");
        setFixedSize(400, 600);

        QTextEdit *editorTexto = new QTextEdit(this);
        editorTexto->setReadOnly(true);

        QFile archivo(rutaArchivo);
        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&archivo);
            editorTexto->setText(in.readAll());
            archivo.close();
        } else {
            editorTexto->setText("Error: No se pudo abrir el archivo.");
        }

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(editorTexto);
        setLayout(layout);
    }
};

class CarritoMovil : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    CarritoMovil(const QPixmap &pixmap, Cliente *cliente, QGraphicsItem *padre = nullptr)
        : QObject(nullptr),
        QGraphicsPixmapItem(pixmap, padre),
        indiceActual(0),
        velocidad(1),
        cliente(cliente) {
        // Crear el texto que mostrará el ID del cliente
        textoId = new QGraphicsTextItem(QString::number(cliente->getId()), this);
        textoId->setDefaultTextColor(Qt::black); // Color del texto
        textoId->setFont(QFont("Arial", 12)); // Fuente y tamaño del texto

        // Centrar el texto sobre el carrito
        QRectF rectCarrito = boundingRect(); // Obtener el rectángulo delimitador del carrito
        textoId->setPos(rectCarrito.width() / 2 - textoId->boundingRect().width() / 2, -20); // Ajusta la posición vertical según sea necesario
    }

    void establecerRuta(const QList<QPointF>& nuevaRuta) {
        ruta = nuevaRuta;
    }

    void advance(int fase) override {
        if (fase && !ruta.isEmpty()) {
            QPointF objetivo = ruta[indiceActual];
            QPointF posicionActual = pos();

            QPointF direccion = objetivo - posicionActual;
            qreal distancia = QLineF(posicionActual, objetivo).length();

            if (distancia > 1) {
                direccion /= distancia;
                setPos(posicionActual + direccion * velocidad);
            } else {
                indiceActual++;
                if (indiceActual >= ruta.size()) {
                    emit carritoCompletado(cliente->getId());
                    if (scene()) scene()->removeItem(this);
                    deleteLater();  // Eliminación segura
                }
            }
        }
    }

    Cliente* getCliente() const { return cliente; }

signals:
    void carritoCompletado(int clienteId);

private:
    QList<QPointF> ruta;
    int indiceActual;
    qreal velocidad;
    Cliente *cliente;
    QGraphicsTextItem *textoId; // Atributo para el texto del ID
};

class GeneradorCarritos : public QObject {
    Q_OBJECT

public:
    GeneradorCarritos(QGraphicsScene *escena, GestorClientes *gestorClientes, QHBoxLayout *layoutClientes, QObject *padre = nullptr)
        : QObject(padre), escena(escena), gestorClientes(gestorClientes), layoutClientes(layoutClientes), generando(false) {
        // Definir la ruta que seguirán los carritos
        ruta = {
            QPointF(10, 10),
            QPointF(10, 275),
            QPointF(50, 275),
            QPointF(50, 90),
            QPointF(250, 90),
            QPointF(250, 380),
            QPointF(450, 380),
            QPointF(450, 90),
            QPointF(650, 90),
            QPointF(650, 300),
            QPointF(935, 300)
        };

        // Configurar el temporizador para generar carritos
        temporizador = new QTimer(this);
        connect(temporizador, &QTimer::timeout, this, &GeneradorCarritos::generarCarrito);
    }

    void iniciarGeneracion() {
        if (!generando) {
            temporizador->start(10000); // Generar un carrito cada 10 segundos
            generando = true;
        }
    }

    void detenerGeneracion() {
        if (generando) {
            temporizador->stop();
            generando = false;
        }
    }

private slots:
    void generarCarrito() {
        QPixmap carritoPixmap("/home/pancho/supermercado_proyecto/carrito.png");
        if (carritoPixmap.isNull()) {
            qDebug() << "Error: No se pudo cargar la imagen del carrito.";
            return;
        }

        QPixmap carritoEscalado = carritoPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        int nuevoId = gestorClientes->obtenerTodosLosClientes().isEmpty() ? 1 : gestorClientes->obtenerTodosLosClientes().last().getId() + 1;
        Cliente cliente(nuevoId, "", "Comprando");
        cliente.generarDatos(); // Generar datos automáticamente (nombre, cédula, teléfono y lista de compra)

        gestorClientes->agregarCliente(cliente);

        actualizarInformacionClientes(layoutClientes, gestorClientes->obtenerTodosLosClientes());

        CarritoMovil *carrito = new CarritoMovil(carritoEscalado, new Cliente(cliente));
        escena->addItem(carrito);
        carrito->establecerRuta(ruta);

        connect(carrito, &CarritoMovil::carritoCompletado, this, [this, cliente]() {
            // Generar la factura
            Factura factura(cliente);
            factura.generarFactura();

            // Guardar la factura en un archivo
            QString rutaArchivo = "/home/pancho/supermercado_proyecto/factura_ultimo_cliente.txt"  + QString::number(cliente.getId()) + ".txt"; // Cambia la ruta al archivo
            factura.guardarFactura(rutaArchivo);

            // Eliminar el cliente del gestor
            gestorClientes->eliminarCliente(cliente.getId());
            actualizarInformacionClientes(layoutClientes, gestorClientes->obtenerTodosLosClientes());
        });
    }

private:
    QGraphicsScene *escena; // Escena donde se mueven los carritos
    QList<QPointF> ruta; // Ruta que siguen los carritos
    QTimer *temporizador; // Temporizador para generar carritos
    bool generando; // Indica si se están generando carritos
    GestorClientes *gestorClientes; // Gestor de clientes
    QHBoxLayout *layoutClientes; // Layout para mostrar la información de los clientes

    // Función para actualizar la información de los clientes en la interfaz
    void actualizarInformacionClientes(QHBoxLayout *layoutClientes, const QList<Cliente> &clientes) {
        // Limpiar el layout actual
        QLayoutItem *item;
        while ((item = layoutClientes->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        // Agregar la información de cada cliente horizontalmente
        for (const Cliente &cliente : clientes) {
            QWidget *widgetCliente = new QWidget;
            QVBoxLayout *layoutCliente = new QVBoxLayout(widgetCliente);

            QString texto = QString("ID: %1\nNombre: %2\nEstado: %3")
                                .arg(cliente.getId())
                                .arg(cliente.getNombre())
                                .arg(cliente.getEstado());

            QLabel *labelCliente = new QLabel(texto);
            layoutCliente->addWidget(labelCliente);

            QPushButton *botonDetalles = new QPushButton("Ver detalles");
            QObject::connect(botonDetalles, &QPushButton::clicked, [cliente]() {
                DialogoCliente *dialogo = new DialogoCliente(cliente);
                dialogo->exec();
            });
            layoutCliente->addWidget(botonDetalles);

            // Agregar el widget del cliente al layout horizontal
            layoutClientes->addWidget(widgetCliente);
        }
    }
};

// Función para cargar un archivo .txt en un QTextEdit
void cargarArchivoTxt(QTextEdit *textEdit, const QString &rutaArchivo) {
    QFile archivo(rutaArchivo);
    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);
        textEdit->setText(in.readAll()); // Cargar el contenido del archivo
        archivo.close();
    } else {
        textEdit->setText("Error: No se pudo abrir el archivo.");
    }
}

int main(int argc, char *argv[]) {
    QApplication aplicacion(argc, argv);

    QGraphicsScene escena;
    escena.setSceneRect(0, 0, 1000, 600);

    GestorClientes gestorClientes;

    // Crear el widget de información de clientes
    QWidget *widgetClientes = new QWidget;
    QHBoxLayout *layoutClientes = new QHBoxLayout(widgetClientes);
    widgetClientes->setMinimumWidth(400);

    // Crear el widget para mostrar el archivo .txt
    QTextEdit *textEditArchivo = new QTextEdit;
    textEditArchivo->setReadOnly(true); // Hacer el QTextEdit de solo lectura

    // Crear un QSplitter vertical
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(widgetClientes); // Parte superior: información de clientes

    // Parte inferior: Archivo .txt
    QWidget *widgetArchivo = new QWidget;
    QVBoxLayout *layoutArchivo = new QVBoxLayout(widgetArchivo);
    layoutArchivo->addWidget(textEditArchivo);
    splitter->addWidget(widgetArchivo);

    // Cargar el archivo .txt
    cargarArchivoTxt(textEditArchivo, "/home/pancho/supermercado_proyecto/factura_ultimo_cliente.txt"); // Cambia la ruta al archivo .txt

    // Configurar el GeneradorCarritos
    GeneradorCarritos generador(&escena, &gestorClientes, layoutClientes);

    // Configurar la imagen de fondo
    QPixmap imagenFondo("/home/pancho/supermercado_proyecto/suelo1.png");
    if (imagenFondo.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen de fondo.";
        return -1;
    }

    QPixmap fondoEscalado = imagenFondo.scaled(306, 204, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QBrush fondo(fondoEscalado);
    fondo.setStyle(Qt::TexturePattern);
    escena.setBackgroundBrush(fondo);

    // Configurar los estantes y la caja
    QPixmap imagenEstante1("/home/pancho/supermercado_proyecto/estante1.png");
    QPixmap imagenEstante2("/home/pancho/supermercado_proyecto/estante2.png");
    QPixmap imagenEstante3("/home/pancho/supermercado_proyecto/estante3.png");
    QPixmap imagenCaja("/home/pancho/supermercado_proyecto/caja.png");

    if (imagenEstante1.isNull() || imagenCaja.isNull() || imagenEstante2.isNull() || imagenEstante3.isNull()) {
        qDebug() << "Error: No se pudieron cargar las imágenes de los elementos.";
        return -1;
    }

    QPixmap estante1Escalado = imagenEstante1.scaled(203, 272, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap estante2Escalado = imagenEstante2.scaled(181, 244, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap estante3Escalado = imagenEstante3.scaled(181, 244, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap cajaEscalado = imagenCaja.scaled(300, 217, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem *estanteItem1 = new QGraphicsPixmapItem(estante1Escalado);
    QGraphicsPixmapItem *estanteItem2 = new QGraphicsPixmapItem(estante2Escalado);
    QGraphicsPixmapItem *estanteItem3 = new QGraphicsPixmapItem(estante3Escalado);
    QGraphicsPixmapItem *estanteItem4 = new QGraphicsPixmapItem(estante1Escalado);
    QGraphicsPixmapItem *estanteItem5 = new QGraphicsPixmapItem(estante2Escalado);
    QGraphicsPixmapItem *estanteItem6 = new QGraphicsPixmapItem(estante3Escalado);
    QGraphicsPixmapItem *cajaItem = new QGraphicsPixmapItem(cajaEscalado);

    estanteItem1->setPos(100, 10);
    estanteItem2->setPos(350, 13);
    estanteItem3->setPos(570, 13);
    estanteItem4->setPos(100, 300);
    estanteItem5->setPos(350, 303);
    estanteItem6->setPos(570, 303);
    cajaItem->setPos(800, 160);

    escena.addItem(estanteItem1);
    escena.addItem(estanteItem2);
    escena.addItem(estanteItem3);
    escena.addItem(estanteItem4);
    escena.addItem(estanteItem5);
    escena.addItem(estanteItem6);
    escena.addItem(cajaItem);

    QGraphicsView vista(&escena);
    vista.setWindowTitle("Generador de Carritos en Qt");
    vista.resize(1000, 600);

    QPushButton *botonGeneracion = new QPushButton("Iniciar Generación");
    botonGeneracion->setCheckable(true);
    QPushButton *botonFacturas = new QPushButton("Mostrar Facturas");

    QHBoxLayout *layoutBotones = new QHBoxLayout;
    layoutBotones->addWidget(botonGeneracion);
    layoutBotones->addWidget(botonFacturas);

    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(layoutBotones);
    layoutPrincipal->addWidget(&vista);

    QHBoxLayout *layoutFinal = new QHBoxLayout;
    layoutFinal->addLayout(layoutPrincipal);
    layoutFinal->addWidget(splitter); // Usar el QSplitter en lugar del widget de clientes

    layoutFinal->setStretch(0, 3);
    layoutFinal->setStretch(1, 1);

    QWidget ventana;
    ventana.setLayout(layoutFinal);
    ventana.setFixedSize(1400, 700);
    ventana.show();

    QObject::connect(botonGeneracion, &QPushButton::toggled, [&](bool checked) {
        if (checked) {
            generador.iniciarGeneracion();
            botonGeneracion->setText("Detener Generación");
        } else {
            generador.detenerGeneracion();
            botonGeneracion->setText("Iniciar Generación");
        }
    });

    QObject::connect(botonFacturas, &QPushButton::clicked, [&]() {
        VentanaFactura *ventanaFactura = new VentanaFactura("/home/pancho/supermercado_proyecto/factura.txt");
        ventanaFactura->exec();
    });

    QTimer temporizador;
    QObject::connect(&temporizador, &QTimer::timeout, &escena, &QGraphicsScene::advance);
    temporizador.start(10);

    return aplicacion.exec();
}

#include "main.moc"
/*


/*
 *
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QList>
#include <QPointF>
#include <QLineF>
#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextEdit>
#include <QStringList>
#include <algorithm>
#include <random>
#include <QMap>
#include <QSplitter>
#include <QCoreApplication>
#include "QXlsx/xlsxdocument.h"
#include <QScopedPointer>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QString>
#include <ctime>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <QVector>

// Declaración de variables globales
std::vector<std::string> cedulasgeneradas;
std::vector<std::string> telefonogeneradas;

// Funciones
const std::string &GenerarCedula() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(4000000, 40000000);

    std::string cedula;
    bool Duplicada;

    do {
        int cedulaNum = distrib(gen);
        cedula = std::to_string(cedulaNum);
        Duplicada = (std::find(cedulasgeneradas.begin(), cedulasgeneradas.end(), cedula) != cedulasgeneradas.end());
    } while (Duplicada);

    cedulasgeneradas.push_back(cedula);
    return cedulasgeneradas.back();
}

const std::string &GenerarTelefono() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib1(500000, 40000000);
    std::uniform_int_distribution<int> distrib2(1, 5);

    int prefijo;
    std::string telefono;
    bool Duplicada;

    do {
        int telefonoNum = distrib1(gen);
        prefijo = distrib2(gen);
        std::stringstream completo;

        switch (prefijo) {
        case 1:
            completo << "0424" << telefonoNum;
            break;
        case 2:
            completo << "0414" << telefonoNum;
            break;
        case 3:
            completo << "0412" << telefonoNum;
            break;
        case 4:
            completo << "0416" << telefonoNum;
            break;
        case 5:
            completo << "0426" << telefonoNum;
            break;
        }

        telefono = completo.str();
        Duplicada = (std::find(telefonogeneradas.begin(), telefonogeneradas.end(), telefono) != telefonogeneradas.end());
    } while (Duplicada);

    telefonogeneradas.push_back(telefono);
    return telefonogeneradas.back();
}

class ManejadorExcel {
public:
    explicit ManejadorExcel(const QString &rutaArchivo)
        : rutaArchivo(rutaArchivo), archivoAbierto(false) {
        xlsx.reset(new QXlsx::Document(rutaArchivo));
    }

    ~ManejadorExcel() {
        if (archivoAbierto) {
            xlsx->save();
        }
    }

    bool abrirArchivo() {
        archivoAbierto = xlsx->load();
        if (!archivoAbierto) {
            qDebug() << "Error: No se pudo abrir el archivo de Excel en la ruta:" << rutaArchivo;
        }
        return archivoAbierto;
    }

    QString leerCelda(int fila, int columna) {
        if (!archivoAbierto) {
            qDebug() << "Error: El archivo no está abierto.";
            return QString();
        }

        QVariant celda = xlsx->read(fila, columna);
        if (celda.isNull() || !celda.isValid()) {
            qDebug() << "Advertencia: Celda en fila" << fila << "columna" << columna << "está vacía o es inválida.";
            return QString();
        }

        return celda.toString();
    }

    bool estaAbierto() const {
        return archivoAbierto;
    }

private:
    QScopedPointer<QXlsx::Document> xlsx;
    QString rutaArchivo;
    bool archivoAbierto;
};

std::string GenerarNombre() {
    ManejadorExcel manejador("Nombres.xlsx");

    if (!manejador.abrirArchivo()) {
        throw std::runtime_error("No se pudo abrir el archivo Excel.");
    }

    int rowCount = 1;
    while (true) {
        QString nombre = manejador.leerCelda(rowCount + 1, 1);
        QString apellido = manejador.leerCelda(rowCount + 1, 2);

        if (nombre.isEmpty() || apellido.isEmpty()) {
            break;
        }

        rowCount++;
    }

    if (rowCount == 0) {
        throw std::runtime_error("No hay datos en el archivo Excel.");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, rowCount);

    int seleccion = distrib(gen);

    QString nombre = manejador.leerCelda(seleccion + 1, 1);
    QString apellido = manejador.leerCelda(seleccion + 1, 2);

    if (nombre.isEmpty() || apellido.isEmpty()) {
        throw std::runtime_error("La fila seleccionada no contiene datos válidos.");
    }

    QString nombreCompleto = nombre + " " + apellido;
    return nombreCompleto.toStdString();
}


// Clase Producto
class Producto {
public:
    QString Nombre;
    QString Descripcion;
    float Precio;
    int Stock;

    Producto(const QString &nombre, const QString &descripcion, float precio, int stock = 0)
        : Nombre(nombre), Descripcion(descripcion), Precio(precio), Stock(stock) {}

    void mostrarDetalles() const {
        qDebug() << "Nombre:" << Nombre
                 << "\nDescripción:" << Descripcion
                 << "\nPrecio:" << Precio
                 << "\nStock:" << Stock;
    }
};

// Clase Carrito
class Carrito {
public:
    QVector<QWeakPointer<Producto>> productos;
    QVector<QSharedPointer<Producto>> productosRegistrados;
    int totalProductos = 0;

    bool agregarProducto(QSharedPointer<Producto> producto, int cantidad) {
        int unidadesAgregadas = 0;

        for (int i = 0; i < cantidad; ++i) {
            if (producto->Stock < 1) {
                qDebug() << "No hay más stock disponible para:" << producto->Nombre;
                break;
            }

            if (productosRegistrados.size() >= 10 && !productosRegistrados.contains(producto)) {
                qDebug() << "Límite de productos distintos alcanzado (10).";
                break;
            }

            int unidadesEnCarrito = unidadesEnCarritoDeProducto(producto);
            if (unidadesEnCarrito + 1 > 5) {
                qDebug() << "Límite de unidades por producto alcanzado (5).";
                break;
            }

            if (totalProductos + 1 > 30) {
                qDebug() << "Límite total de productos en el carrito alcanzado (30).";
                break;
            }

            producto->Stock--;
            productos.append(QWeakPointer<Producto>(producto));
            totalProductos++;
            unidadesAgregadas++;

            if (!productosRegistrados.contains(producto)) {
                productosRegistrados.append(producto);
            }
        }

        if (unidadesAgregadas > 0) {
            qDebug() << "Producto agregado:" << producto->Nombre << ", Cantidad:" << unidadesAgregadas;
            return true;
        } else {
            qDebug() << "No se pudo agregar ninguna unidad de:" << producto->Nombre;
            return false;
        }
    }

    QString sacarUltimoProducto() {
        if (productos.isEmpty()) {
            qDebug() << "El carrito está vacío.";
            return QString();
        }

        QWeakPointer<Producto> ultimoProductoWeak = productos.last();
        productos.removeLast();
        totalProductos--;

        if (auto ultimoProducto = ultimoProductoWeak.toStrongRef()) {
            qDebug() << "Producto sacado:" << ultimoProducto->Nombre;
            return ultimoProducto->Nombre;
        } else {
            qDebug() << "El producto ya no está disponible.";
            return QString();
        }
    }

    void generarFactura() {
        QFile archivo("factura.txt");
        if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Error: No se pudo abrir el archivo para guardar la factura.";
            return;
        }

        QTextStream out(&archivo);
        out << "--- Factura ---\n";
        float totalPagar = 0;

        while (!productos.isEmpty()) {
            QString ultimoProducto = sacarUltimoProducto();

            for (const auto &producto : productosRegistrados) {
                if (producto->Nombre == ultimoProducto) {
                    float precio = producto->Precio;
                    out << "Producto: " << ultimoProducto << ", Precio unitario: " << precio << "\n";
                    totalPagar += precio;
                    break;
                }
            }
        }

        out << "Total a pagar: " << totalPagar << "\n";
        out << "---------------\n";

        archivo.close();

        productos.clear();
        productosRegistrados.clear();
        totalProductos = 0;
    }

private:
    int unidadesEnCarritoDeProducto(QSharedPointer<Producto> producto) {
        int count = 0;
        for (const auto &weakProducto : productos) {
            if (auto sharedProducto = weakProducto.toStrongRef()) {
                if (sharedProducto == producto) {
                    count++;
                }
            }
        }
        return count;
    }
};

// Clase Cliente
class Cliente {
public:
    std::string Nombre;
    const std::string &Cedula;
    const std::string &Telefono;

    Cliente()
        : Cedula(GenerarCedula()), Telefono(GenerarTelefono()), Nombre(GenerarNombre()) {}

    void presentarme() const {
        std::cout << "Nombre: " << Nombre << "\nCédula: " << Cedula << "\nTeléfono: " << Telefono << std::endl;
    }
};

// Función para leer productos desde el archivo Excel
std::vector<Producto> leerProductosDesdeExcel(const QString &rutaArchivo) {
    ManejadorExcel manejador(rutaArchivo);

    if (!manejador.abrirArchivo()) {
        throw std::runtime_error("No se pudo abrir el archivo Excel.");
    }

    std::vector<Producto> productos;
    int fila = 2;

    while (true) {
        QString nombre = manejador.leerCelda(fila, 1);
        QString descripcion = manejador.leerCelda(fila, 2);
        QString precioStr = manejador.leerCelda(fila, 3);

        if (nombre.isEmpty() || descripcion.isEmpty() || precioStr.isEmpty()) {
            break;
        }

        bool conversionOk;
        float precio = precioStr.toFloat(&conversionOk);

        if (!conversionOk) {
            throw std::runtime_error("Error al convertir el precio a float.");
        }

        productos.emplace_back(nombre, descripcion, precio);
        fila++;
    }

    return productos;
}

// Clase para representar a un cliente en la simulación
class ClienteSimulacion {
public:
    ClienteSimulacion() : id(0), nombre(""), estado(""), listaCompra(QStringList()) {}
    ClienteSimulacion(int id, const QString &nombre, const QString &estado, const QStringList &listaCompra = QStringList())
        : id(id), nombre(nombre), estado(estado), listaCompra(listaCompra) {}

    int getId() const { return id; }
    QString getNombre() const { return nombre; }
    QString getEstado() const { return estado; }
    QStringList getListaCompra() const { return listaCompra; }

    void setEstado(const QString &nuevoEstado) { estado = nuevoEstado; }
    void agregarProducto(const QString &producto) { listaCompra.append(producto); }
    void eliminarProducto(const QString &producto) { listaCompra.removeAll(producto); }

private:
    int id;
    QString nombre;
    QString estado;
    QStringList listaCompra;
};

// Clase para gestionar múltiples clientes
class GestorClientes {
public:
    void agregarCliente(const ClienteSimulacion &cliente) {
        clientes[cliente.getId()] = cliente;
    }

    void eliminarCliente(int id) {
        clientes.remove(id);
    }

    ClienteSimulacion obtenerCliente(int id) const {
        return clientes.value(id);
    }

    QList<ClienteSimulacion> obtenerTodosLosClientes() const {
        return clientes.values();
    }

    bool existeCliente(int id) const {
        return clientes.contains(id);
    }

private:
    QMap<int, ClienteSimulacion> clientes;
};

// Clase para el cuadro de diálogo de detalles del cliente
class DialogoCliente : public QDialog {
public:
    DialogoCliente(const ClienteSimulacion &cliente, QWidget *padre = nullptr)
        : QDialog(padre) {
        setWindowTitle("Detalles del Cliente");
        setFixedSize(400, 300);

        QLabel *labelId = new QLabel("ID: " + QString::number(cliente.getId()), this);
        QLabel *labelNombre = new QLabel("Nombre: " + cliente.getNombre(), this);
        QLabel *labelEstado = new QLabel("Estado: " + cliente.getEstado(), this);

        QTextEdit *textEditListaCompra = new QTextEdit(this);
        textEditListaCompra->setReadOnly(true);
        textEditListaCompra->setText("Lista de la compra:\n" + cliente.getListaCompra().join("\n"));

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(labelId);
        layout->addWidget(labelNombre);
        layout->addWidget(labelEstado);
        layout->addWidget(textEditListaCompra);

        setLayout(layout);
    }
};

// Clase para la ventana de factura
class VentanaFactura : public QDialog {
public:
    VentanaFactura(const QString &rutaArchivo, QWidget *padre = nullptr) : QDialog(padre) {
        setWindowTitle("Detalles de la Factura");
        setFixedSize(400, 300);

        QTextEdit *editorTexto = new QTextEdit(this);
        editorTexto->setReadOnly(true);

        QFile archivo(rutaArchivo);
        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&archivo);
            editorTexto->setText(in.readAll());
            archivo.close();
        } else {
            editorTexto->setText("Error: No se pudo abrir el archivo.");
        }

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(editorTexto);
        setLayout(layout);
    }
};

class CarritoMovil : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    CarritoMovil(const QPixmap &pixmap, ClienteSimulacion *cliente, QSharedPointer<Carrito> carrito, QGraphicsItem *padre = nullptr)
        : QObject(nullptr),
        QGraphicsPixmapItem(pixmap, padre),
        indiceActual(0),
        velocidad(1),
        cliente(cliente),
        carrito(carrito) {
        textoId = new QGraphicsTextItem(QString::number(cliente->getId()), this);
        textoId->setDefaultTextColor(Qt::black);
        textoId->setFont(QFont("Arial", 12));
        textoId->setPos(boundingRect().width() / 2 - textoId->boundingRect().width() / 2, -20);
    }

    void establecerRuta(const QList<QPointF>& nuevaRuta) {
        ruta = nuevaRuta;
    }

    void advance(int fase) override {
        if (fase && !ruta.isEmpty()) {
            QPointF objetivo = ruta[indiceActual];
            QPointF posicionActual = pos();

            QPointF direccion = objetivo - posicionActual;
            qreal distancia = QLineF(posicionActual, objetivo).length();

            if (distancia > 1) {
                direccion /= distancia;
                setPos(posicionActual + direccion * velocidad);
            } else {
                indiceActual++;
                if (indiceActual >= ruta.size()) {
                    emit carritoCompletado(cliente->getId());
                    if (scene()) scene()->removeItem(this);
                    deleteLater();
                }
            }
        }
    }

    ClienteSimulacion* getCliente() const { return cliente; }
    QSharedPointer<Carrito> getCarrito() const { return carrito; }

signals:
    void carritoCompletado(int clienteId);

private:
    QList<QPointF> ruta;
    int indiceActual;
    qreal velocidad;
    ClienteSimulacion *cliente;
    QGraphicsTextItem *textoId;
    QSharedPointer<Carrito> carrito;
};

// Función para actualizar la información de los clientes en la interfaz
void actualizarInformacionClientes(QHBoxLayout *layoutClientes, const QList<ClienteSimulacion> &clientes) {
    QLayoutItem *item;
    while ((item = layoutClientes->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    for (const ClienteSimulacion &cliente : clientes) {
        QWidget *widgetCliente = new QWidget;
        QVBoxLayout *layoutCliente = new QVBoxLayout(widgetCliente);

        QString texto = QString("ID: %1\nNombre: %2\nEstado: %3")
                            .arg(cliente.getId())
                            .arg(cliente.getNombre())
                            .arg(cliente.getEstado());

        QLabel *labelCliente = new QLabel(texto);
        layoutCliente->addWidget(labelCliente);

        QPushButton *botonDetalles = new QPushButton("Ver detalles");
        QObject::connect(botonDetalles, &QPushButton::clicked, [cliente]() {
            DialogoCliente *dialogo = new DialogoCliente(cliente);
            dialogo->exec();
        });
        layoutCliente->addWidget(botonDetalles);

        layoutClientes->addWidget(widgetCliente);
    }
}

class GeneradorCarritos : public QObject {
    Q_OBJECT

public:
    GeneradorCarritos(QGraphicsScene *escena, GestorClientes *gestorClientes, QHBoxLayout *layoutClientes, QObject *padre = nullptr)
        : QObject(padre), escena(escena), gestorClientes(gestorClientes), layoutClientes(layoutClientes), generando(false) {
        ruta = {
            QPointF(10, 10),
            QPointF(10, 275),
            QPointF(50, 275),
            QPointF(50, 90),
            QPointF(250, 90),
            QPointF(250, 380),
            QPointF(450, 380),
            QPointF(450, 90),
            QPointF(650, 90),
            QPointF(650, 300),
            QPointF(935, 300)
        };

        temporizador = new QTimer(this);
        connect(temporizador, &QTimer::timeout, this, &GeneradorCarritos::generarCarrito);
    }

    void iniciarGeneracion() {
        if (!generando) {
            temporizador->start(10000);
            generando = true;
        }
    }

    void detenerGeneracion() {
        if (generando) {
            temporizador->stop();
            generando = false;
        }
    }

private slots:
    void generarCarrito() {
        QPixmap carritoPixmap("/home/pancho/supermercado_proyecto/carrito.png");
        if (carritoPixmap.isNull()) {
            qDebug() << "Error: No se pudo cargar la imagen del carrito.";
            return;
        }

        QPixmap carritoEscalado = carritoPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        int nuevoId = gestorClientes->obtenerTodosLosClientes().isEmpty() ? 1 : gestorClientes->obtenerTodosLosClientes().last().getId() + 1;
        QString nombreCliente = "Cliente " + QString::number(nuevoId);
        QString estadoCliente = "Comprando";

        QStringList productosDisponibles = {"Manzanas", "Pan", "Leche", "Arroz", "Frijoles", "Aceite", "Pollo", "Papas", "Cebolla"};
        QStringList listaCompra;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 5);
        int numProductos = dis(gen);

        std::sample(productosDisponibles.begin(), productosDisponibles.end(), std::back_inserter(listaCompra),
                    numProductos, std::mt19937{std::random_device{}()});

        ClienteSimulacion cliente(nuevoId, nombreCliente, estadoCliente, listaCompra);
        gestorClientes->agregarCliente(cliente);

        actualizarInformacionClientes(layoutClientes, gestorClientes->obtenerTodosLosClientes());

        QSharedPointer<Carrito> carrito(new Carrito());
        for (const auto &productoNombre : listaCompra) {
            // Aquí deberías buscar el producto en tu lista de productos y agregarlo al carrito
            // Por simplicidad, se asume que el producto existe y se agrega
            // Ejemplo: carrito->agregarProducto(producto, 1);
        }

        CarritoMovil *carritoMovil = new CarritoMovil(carritoEscalado, new ClienteSimulacion(cliente), carrito);
        escena->addItem(carritoMovil);
        carritoMovil->establecerRuta(ruta);

        connect(carritoMovil, &CarritoMovil::carritoCompletado, this, [this](int clienteId) {
            gestorClientes->eliminarCliente(clienteId);
            actualizarInformacionClientes(layoutClientes, gestorClientes->obtenerTodosLosClientes());
        });
    }

private:
    QGraphicsScene *escena;
    QList<QPointF> ruta;
    QTimer *temporizador;
    bool generando;
    GestorClientes *gestorClientes;
    QHBoxLayout *layoutClientes;
};

int main(int argc, char *argv[]) {
    QApplication aplicacion(argc, argv);

    QGraphicsScene escena;
    escena.setSceneRect(0, 0, 1000, 600);

    GestorClientes gestorClientes;

    // Crear el widget de información de clientes
    QWidget *widgetClientes = new QWidget;
    QHBoxLayout *layoutClientes = new QHBoxLayout(widgetClientes);
    widgetClientes->setMinimumWidth(400);

    // Crear el widget de factura
    QWidget *widgetFactura = new QWidget;
    QVBoxLayout *layoutFactura = new QVBoxLayout(widgetFactura);
    QTextEdit *editorFactura = new QTextEdit(widgetFactura);
    editorFactura->setReadOnly(true);
    layoutFactura->addWidget(editorFactura);

    // Crear un QSplitter vertical
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(widgetClientes); // Parte superior: información de clientes
    splitter->addWidget(widgetFactura);  // Parte inferior: factura

    // Configurar el GeneradorCarritos
    GeneradorCarritos generador(&escena, &gestorClientes, layoutClientes);

    // Configurar la imagen de fondo
    QPixmap imagenFondo("/home/pancho/supermercado_proyecto/suelo1.png");
    if (imagenFondo.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen de fondo.";
        return -1;
    }

    QPixmap fondoEscalado = imagenFondo.scaled(306, 204, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QBrush fondo(fondoEscalado);
    fondo.setStyle(Qt::TexturePattern);
    escena.setBackgroundBrush(fondo);

    // Configurar los estantes y la caja
    QPixmap imagenEstante1("/home/pancho/supermercado_proyecto/estante1.png");
    QPixmap imagenEstante2("/home/pancho/supermercado_proyecto/estante2.png");
    QPixmap imagenEstante3("/home/pancho/supermercado_proyecto/estante3.png");
    QPixmap imagenCaja("/home/pancho/supermercado_proyecto/caja.png");

    if (imagenEstante1.isNull() || imagenCaja.isNull() || imagenEstante2.isNull() || imagenEstante3.isNull()) {
        qDebug() << "Error: No se pudieron cargar las imágenes de los elementos.";
        return -1;
    }

    QPixmap estante1Escalado = imagenEstante1.scaled(203, 272, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap estante2Escalado = imagenEstante2.scaled(181, 244, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap estante3Escalado = imagenEstante3.scaled(181, 244, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap cajaEscalado = imagenCaja.scaled(300, 217, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem *estanteItem1 = new QGraphicsPixmapItem(estante1Escalado);
    QGraphicsPixmapItem *estanteItem2 = new QGraphicsPixmapItem(estante2Escalado);
    QGraphicsPixmapItem *estanteItem3 = new QGraphicsPixmapItem(estante3Escalado);
    QGraphicsPixmapItem *estanteItem4 = new QGraphicsPixmapItem(estante1Escalado);
    QGraphicsPixmapItem *estanteItem5 = new QGraphicsPixmapItem(estante2Escalado);
    QGraphicsPixmapItem *estanteItem6 = new QGraphicsPixmapItem(estante3Escalado);
    QGraphicsPixmapItem *cajaItem = new QGraphicsPixmapItem(cajaEscalado);

    estanteItem1->setPos(100, 10);
    estanteItem2->setPos(350, 13);
    estanteItem3->setPos(570, 13);
    estanteItem4->setPos(100, 300);
    estanteItem5->setPos(350, 303);
    estanteItem6->setPos(570, 303);
    cajaItem->setPos(800, 160);

    escena.addItem(estanteItem1);
    escena.addItem(estanteItem2);
    escena.addItem(estanteItem3);
    escena.addItem(estanteItem4);
    escena.addItem(estanteItem5);
    escena.addItem(estanteItem6);
    escena.addItem(cajaItem);

    QGraphicsView vista(&escena);
    vista.setWindowTitle("Generador de Carritos en Qt");
    vista.resize(1000, 600);

    QPushButton *botonGeneracion = new QPushButton("Iniciar Generación");
    botonGeneracion->setCheckable(true);
    QPushButton *botonFacturas = new QPushButton("Mostrar Facturas");

    QHBoxLayout *layoutBotones = new QHBoxLayout;
    layoutBotones->addWidget(botonGeneracion);
    layoutBotones->addWidget(botonFacturas);

    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(layoutBotones);
    layoutPrincipal->addWidget(&vista);

    QHBoxLayout *layoutFinal = new QHBoxLayout;
    layoutFinal->addLayout(layoutPrincipal);
    layoutFinal->addWidget(splitter); // Usar el QSplitter en lugar del widget de clientes

    layoutFinal->setStretch(0, 3);
    layoutFinal->setStretch(1, 1);

    QWidget ventana;
    ventana.setLayout(layoutFinal);
    ventana.setFixedSize(1400, 700);
    ventana.show();

    QObject::connect(botonGeneracion, &QPushButton::toggled, [&](bool checked) {
        if (checked) {
            generador.iniciarGeneracion();
            botonGeneracion->setText("Detener Generación");
        } else {
            generador.detenerGeneracion();
            botonGeneracion->setText("Iniciar Generación");
        }
    });

    QObject::connect(botonFacturas, &QPushButton::clicked, [&]() {
        VentanaFactura *ventanaFactura = new VentanaFactura("/home/pancho/supermercado_proyecto/factura.txt");
        ventanaFactura->exec();
    });

    QTimer temporizador;
    QObject::connect(&temporizador, &QTimer::timeout, &escena, &QGraphicsScene::advance);
    temporizador.start(10);

    return aplicacion.exec();
}

#include "main.moc"
*/
/*
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QList>
#include <QPointF>
#include <QLineF>
#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextEdit>
#include <QStringList>
#include <algorithm>
#include <random>
#include <QMap>
#include <QSplitter>
#include <QCoreApplication>
#include "xlsxdocument.h"
#include <QScopedPointer>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QString>
#include <ctime>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <QVector>

    /* Declaración de variables globales
    std::vector<std::string> cedulasgeneradas;
std::vector<std::string> telefonogeneradas;

std::string GenerarNombre() {
    // Lista de nombres y apellidos
    std::vector<std::string> nombres = {"Juan", "María", "Carlos", "Ana", "Luis"};
    std::vector<std::string> apellidos = {"Pérez", "Gómez", "López", "Martínez", "Rodríguez", "Rivas"};

    // Generar índices aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribNombre(0, nombres.size() - 1);
    std::uniform_int_distribution<> distribApellido(0, apellidos.size() - 1);

    // Seleccionar un nombre y un apellido aleatorio
    std::string nombreCompleto = nombres[distribNombre(gen)] + " " + apellidos[distribApellido(gen)];
    return nombreCompleto;
}

std::string GenerarCedula() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(4000000, 40000000);

    std::string cedula;
    bool Duplicada;

    do {
        int cedulaNum = distrib(gen);
        cedula = std::to_string(cedulaNum);
        Duplicada = (std::find(cedulasgeneradas.begin(), cedulasgeneradas.end(), cedula) != cedulasgeneradas.end());
    } while (Duplicada);

    cedulasgeneradas.push_back(cedula);
    return cedulasgeneradas.back();
}

std::string GenerarTelefono() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib1(500000, 40000000);
    std::uniform_int_distribution<int> distrib2(1, 5);

    int prefijo;
    std::string telefono;
    bool Duplicada;

    do {
        int telefonoNum = distrib1(gen);
        prefijo = distrib2(gen);
        std::stringstream completo;

        switch (prefijo) {
        case 1:
            completo << "0424" << telefonoNum;
            break;
        case 2:
            completo << "0414" << telefonoNum;
            break;
        case 3:
            completo << "0412" << telefonoNum;
            break;
        case 4:
            completo << "0416" << telefonoNum;
            break;
        case 5:
            completo << "0426" << telefonoNum;
            break;
        }

        telefono = completo.str();
        Duplicada = (std::find(telefonogeneradas.begin(), telefonogeneradas.end(), telefono) != telefonogeneradas.end());
    } while (Duplicada);

    telefonogeneradas.push_back(telefono);
    return telefonogeneradas.back();
}

class Cliente {
public:
    Cliente() : id(0), nombre(""), estado("") {}
    Cliente(int id, const QString &nombre, const QString &estado, const QMap<QString, int> &listaCompra = QMap<QString, int>())
        : id(id), nombre(nombre), estado(estado), listaCompra(listaCompra) {}

    int getId() const { return id; }
    QString getNombre() const { return nombre; }
    QString getEstado() const { return estado; }
    QMap<QString, int> getListaCompra() const { return listaCompra; }
    QString getCedula() const { return cedula; }
    QString getTelefono() const { return telefono; }

    void setEstado(const QString &nuevoEstado) { estado = nuevoEstado; }

    // Método para agregar un producto con límites
    void agregarProducto(const QString &producto, int cantidad) {
        if (listaCompra.contains(producto)) {
            // Verificar que no se exceda el límite de 3 por producto
            if (listaCompra[producto] + cantidad <= 3) {
                listaCompra[producto] += cantidad;
            } else {
                listaCompra[producto] = 3; // Establecer el máximo permitido
            }
        } else {
            // Verificar que no se exceda el límite de 3 por producto
            if (cantidad <= 3) {
                listaCompra[producto] = cantidad;
            } else {
                listaCompra[producto] = 3; // Establecer el máximo permitido
            }
        }
    }

    void eliminarProducto(const QString &producto) { listaCompra.remove(producto); }

    void generarDatos() {
        nombre = QString::fromStdString(GenerarNombre());
        cedula = QString::fromStdString(GenerarCedula());
        telefono = QString::fromStdString(GenerarTelefono());
        generarListaCompra();
    }

    // Método para generar la lista de compra con límites
    void generarListaCompra() {
        QStringList productosDisponibles = {"Manzanas", "Pan", "Leche", "Arroz", "Frijoles", "Aceite", "Pollo", "Papas", "Cebolla"};

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> disProductos(1, 5); // Número aleatorio de productos (1 a 5)
        std::uniform_int_distribution<> disCantidad(1, 3); // Cantidad aleatoria por producto (1 a 3)

        int totalProductos = 0; // Contador para la suma total de productos
        int numProductos = disProductos(gen);

        // Seleccionar productos aleatorios con cantidades
        for (int i = 0; i < numProductos; ++i) {
            if (totalProductos >= 30) break; // No superar el límite de 30 productos

            QString producto = productosDisponibles[std::uniform_int_distribution<>(0, productosDisponibles.size() - 1)(gen)];
            int cantidad = disCantidad(gen);

            // Verificar que no se exceda el límite de 30 productos en total
            if (totalProductos + cantidad > 30) {
                cantidad = 30 - totalProductos; // Ajustar la cantidad para no superar el límite
            }

            // Agregar el producto con la cantidad ajustada
            agregarProducto(producto, cantidad);
            totalProductos += cantidad;
        }
    }

private:
    int id;
    QString nombre;
    QString estado;
    QMap<QString, int> listaCompra; // Lista de compra con cantidades
    QString cedula;
    QString telefono;
};

// Clase para gestionar múltiples clientes
class GestorClientes {
public:
    void agregarCliente(const Cliente &cliente) {
        clientes[cliente.getId()] = cliente;
    }

    void eliminarCliente(int id) {
        clientes.remove(id);
    }

    Cliente obtenerCliente(int id) const {
        return clientes.value(id);
    }

    QList<Cliente> obtenerTodosLosClientes() const {
        return clientes.values();
    }

    bool existeCliente(int id) const {
        return clientes.contains(id);
    }

private:
    QMap<int, Cliente> clientes;
};

class Factura {
public:
    Factura(const Cliente &cliente) : cliente(cliente) {}

    void generarFactura() {
        // Crear el contenido de la factura
        contenido = "Factura para: " + cliente.getNombre() + "\n";
        contenido += "Cédula: " + cliente.getCedula() + "\n";
        contenido += "Teléfono: " + cliente.getTelefono() + "\n\n";
        contenido += "Productos:\n";

        double total = 0.0;
        QMap<QString, int> listaCompra = cliente.getListaCompra();
        for (auto it = listaCompra.begin(); it != listaCompra.end(); ++it) {
            double precio = obtenerPrecioProducto(it.key()); // Obtener el precio del producto
            double subtotal = precio * it.value();
            contenido += it.key() + " x " + QString::number(it.value()) + " = $" + QString::number(subtotal, 'f', 2) + "\n";
            total += subtotal;
        }

        contenido += "\nTotal: $" + QString::number(total, 'f', 2) + "\n";
    }

    void guardarFactura(const QString &rutaArchivo) {
        QFile archivo(rutaArchivo);
        if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&archivo);
            out << contenido;
            archivo.close();
        } else {
            qDebug() << "Error: No se pudo guardar la factura.";
        }
    }

    QString getContenido() const { return contenido; }

private:
    Cliente cliente;
    QString contenido;

    // Método para obtener el precio de un producto (puedes cargarlo desde una base de datos o archivo)
    double obtenerPrecioProducto(const QString &producto) {
        QMap<QString, double> precios = {
            {"Manzanas", 1.50},
            {"Pan", 0.80},
            {"Leche", 1.20},
            {"Arroz", 2.00},
            {"Frijoles", 1.80},
            {"Aceite", 3.50},
            {"Pollo", 5.00},
            {"Papas", 1.00},
            {"Cebolla", 0.50}
        };
        return precios.value(producto, 0.0); // Retorna 0.0 si el producto no está en la lista
    }
};

// Clase para el cuadro de diálogo de detalles del cliente
class DialogoCliente : public QDialog {
public:
    DialogoCliente(const Cliente &cliente, QWidget *padre = nullptr)
        : QDialog(padre) {
        setWindowTitle("Detalles del Cliente");
        setFixedSize(400, 300);

        QLabel *labelId = new QLabel("ID: " + QString::number(cliente.getId()), this);
        QLabel *labelNombre = new QLabel("Nombre: " + cliente.getNombre(), this);
        QLabel *labelEstado = new QLabel("Estado: " + cliente.getEstado(), this);
        QLabel *labelCedula = new QLabel("Cédula: " + cliente.getCedula(), this);
        QLabel *labelTelefono = new QLabel("Teléfono: " + cliente.getTelefono(), this);

        QTextEdit *textEditListaCompra = new QTextEdit(this);
        textEditListaCompra->setReadOnly(true);

        QString listaCompraTexto = "Lista de la compra:\n";
        QMap<QString, int> listaCompra = cliente.getListaCompra();
        for (auto it = listaCompra.begin(); it != listaCompra.end(); ++it) {
            listaCompraTexto += it.key() + ": " + QString::number(it.value()) + "\n";
        }

        textEditListaCompra->setText(listaCompraTexto);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(labelId);
        layout->addWidget(labelNombre);
        layout->addWidget(labelEstado);
        layout->addWidget(labelCedula);
        layout->addWidget(labelTelefono);
        layout->addWidget(textEditListaCompra);

        setLayout(layout);
    }
};

class CarritoMovil : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    CarritoMovil(const QPixmap &pixmap, Cliente *cliente, QGraphicsItem *padre = nullptr)
        : QObject(nullptr),
        QGraphicsPixmapItem(pixmap, padre),
        indiceActual(0),
        velocidad(1),
        cliente(cliente) {
        // Crear el texto que mostrará el ID del cliente
        textoId = new QGraphicsTextItem(QString::number(cliente->getId()), this);
        textoId->setDefaultTextColor(Qt::black); // Color del texto
        textoId->setFont(QFont("Arial", 12)); // Fuente y tamaño del texto

        // Centrar el texto sobre el carrito
        QRectF rectCarrito = boundingRect(); // Obtener el rectángulo delimitador del carrito
        textoId->setPos(rectCarrito.width() / 2 - textoId->boundingRect().width() / 2, -20); // Ajusta la posición vertical según sea necesario
    }

    void establecerRuta(const QList<QPointF>& nuevaRuta) {
        ruta = nuevaRuta;
    }

    void advance(int fase) override {
        if (fase && !ruta.isEmpty()) {
            QPointF objetivo = ruta[indiceActual];
            QPointF posicionActual = pos();

            QPointF direccion = objetivo - posicionActual;
            qreal distancia = QLineF(posicionActual, objetivo).length();

            if (distancia > 1) {
                direccion /= distancia;
                setPos(posicionActual + direccion * velocidad);
            } else {
                indiceActual++;
                if (indiceActual >= ruta.size()) {
                    emit carritoCompletado(cliente->getId());
                    if (scene()) scene()->removeItem(this);
                    deleteLater();  // Eliminación segura
                }
            }
        }
    }

    Cliente* getCliente() const { return cliente; }

signals:
    void carritoCompletado(int clienteId);

private:
    QList<QPointF> ruta;
    int indiceActual;
    qreal velocidad;
    Cliente *cliente;
    QGraphicsTextItem *textoId; // Atributo para el texto del ID
};

// Función para actualizar la información de los clientes en la interfaz
void actualizarInformacionClientes(QHBoxLayout *layoutClientes, const QList<Cliente> &clientes) {
    // Limpiar el layout actual
    QLayoutItem *item;
    while ((item = layoutClientes->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Agregar la información de cada cliente horizontalmente
    for (const Cliente &cliente : clientes) {
        QWidget *widgetCliente = new QWidget;
        QVBoxLayout *layoutCliente = new QVBoxLayout(widgetCliente);

        QString texto = QString("ID: %1\nNombre: %2\nEstado: %3")
                            .arg(cliente.getId())
                            .arg(cliente.getNombre())
                            .arg(cliente.getEstado());

        QLabel *labelCliente = new QLabel(texto);
        layoutCliente->addWidget(labelCliente);

        QPushButton *botonDetalles = new QPushButton("Ver detalles");
        QObject::connect(botonDetalles, &QPushButton::clicked, [cliente]() {
            DialogoCliente *dialogo = new DialogoCliente(cliente);
            dialogo->exec();
        });
        layoutCliente->addWidget(botonDetalles);

        // Agregar el widget del cliente al layout horizontal
        layoutClientes->addWidget(widgetCliente);
    }
}

class GeneradorCarritos : public QObject {
    Q_OBJECT

public:
    GeneradorCarritos(QGraphicsScene *escena, GestorClientes *gestorClientes, QVBoxLayout *layoutClientes, QTextEdit *textEditFactura, QObject *padre = nullptr)
        : QObject(padre), escena(escena), gestorClientes(gestorClientes), layoutClientes(layoutClientes), textEditFactura(textEditFactura), generando(false) {
        // Definir la ruta que seguirán los carritos
        ruta = {
            QPointF(10, 10),
            QPointF(10, 275),
            QPointF(50, 275),
            QPointF(50, 90),
            QPointF(250, 90),
            QPointF(250, 380),
            QPointF(450, 380),
            QPointF(450, 90),
            QPointF(650, 90),
            QPointF(650, 300),
            QPointF(935, 300)
        };

        // Configurar el temporizador para generar carritos
        temporizador = new QTimer(this);
        connect(temporizador, &QTimer::timeout, this, &GeneradorCarritos::generarCarrito);
    }

    void iniciarGeneracion() {
        if (!generando) {
            temporizador->start(10000); // Generar un carrito cada 10 segundos
            generando = true;
        }
    }

    void detenerGeneracion() {
        if (generando) {
            temporizador->stop();
            generando = false;
        }
    }

private slots:
    void generarCarrito() {
        QPixmap carritoPixmap("/home/pancho/supermercado_proyecto/carrito.png");
        if (carritoPixmap.isNull()) {
            qDebug() << "Error: No se pudo cargar la imagen del carrito.";
            return;
        }

        QPixmap carritoEscalado = carritoPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        int nuevoId = gestorClientes->obtenerTodosLosClientes().isEmpty() ? 1 : gestorClientes->obtenerTodosLosClientes().last().getId() + 1;
        Cliente cliente(nuevoId, "", "Comprando");
        cliente.generarDatos(); // Generar datos automáticamente (nombre, cédula, teléfono y lista de compra)

        gestorClientes->agregarCliente(cliente);

        actualizarInformacionClientes(layoutClientes, gestorClientes->obtenerTodosLosClientes());

        CarritoMovil *carrito = new CarritoMovil(carritoEscalado, new Cliente(cliente));
        escena->addItem(carrito);
        carrito->establecerRuta(ruta);

        connect(carrito, &CarritoMovil::carritoCompletado, this, [this, cliente]() {
            // Generar la factura
            Factura factura(cliente);
            factura.generarFactura();

            // Actualizar el QTextEdit con el contenido de la factura
            textEditFactura->setText(factura.getContenido());

            // Guardar la factura en un archivo (opcional)
            QString rutaArchivo = "/home/pancho/supermercado_proyecto/factura_cliente_" + QString::number(cliente.getId()) + ".txt";
            factura.guardarFactura(rutaArchivo);

            // Eliminar el cliente del gestor
            gestorClientes->eliminarCliente(cliente.getId());
            actualizarInformacionClientes(layoutClientes, gestorClientes->obtenerTodosLosClientes());
        });
    }

private:
    QGraphicsScene *escena; // Escena donde se mueven los carritos
    QList<QPointF> ruta; // Ruta que siguen los carritos
    QTimer *temporizador; // Temporizador para generar carritos
    bool generando; // Indica si se están generando carritos
    GestorClientes *gestorClientes; // Gestor de clientes
    QVBoxLayout *layoutClientes; // Layout para mostrar la información de los clientes
    QTextEdit *textEditFactura; // QTextEdit para mostrar la factura

    // Función para actualizar la información de los clientes en la interfaz
    void actualizarInformacionClientes(QVBoxLayout *layoutClientes, const QList<Cliente> &clientes) {
        // Limpiar el layout actual
        QLayoutItem *item;
        while ((item = layoutClientes->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        // Agregar la información de cada cliente horizontalmente
        for (const Cliente &cliente : clientes) {
            QWidget *widgetCliente = new QWidget;
            QVBoxLayout *layoutCliente = new QVBoxLayout(widgetCliente);

            QString texto = QString("ID: %1\nNombre: %2\nEstado: %3")
                                .arg(cliente.getId())
                                .arg(cliente.getNombre())
                                .arg(cliente.getEstado());

            QLabel *labelCliente = new QLabel(texto);
            layoutCliente->addWidget(labelCliente);

            QPushButton *botonDetalles = new QPushButton("Ver detalles");
            QObject::connect(botonDetalles, &QPushButton::clicked, [cliente]() {
                DialogoCliente *dialogo = new DialogoCliente(cliente);
                dialogo->exec();
            });
            layoutCliente->addWidget(botonDetalles);

            // Agregar el widget del cliente al layout horizontal
            layoutClientes->addWidget(widgetCliente);
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication aplicacion(argc, argv);

    QGraphicsScene escena;
    escena.setSceneRect(0, 0, 1000, 600);

    GestorClientes gestorClientes;

    // Crear el widget de información de clientes
    QWidget *widgetClientes = new QWidget;
    QVBoxLayout *layoutClientes = new QVBoxLayout(widgetClientes);
    widgetClientes->setMinimumWidth(400);

    // Crear un QTextEdit para mostrar la factura
    QTextEdit *textEditFactura = new QTextEdit;
    textEditFactura->setReadOnly(true); // Hacer el QTextEdit de solo lectura

    // Agregar el QTextEdit al layout de clientes
    layoutClientes->addWidget(textEditFactura);

    // Configurar el GeneradorCarritos
    GeneradorCarritos generador(&escena, &gestorClientes, layoutClientes, textEditFactura);

    // Configurar la imagen de fondo
    QPixmap imagenFondo("/home/pancho/supermercado_proyecto/suelo1.png");
    if (imagenFondo.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen de fondo.";
        return -1;
    }

    QPixmap fondoEscalado = imagenFondo.scaled(306, 204, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QBrush fondo(fondoEscalado);
    fondo.setStyle(Qt::TexturePattern);
    escena.setBackgroundBrush(fondo);

    // Configurar los estantes y la caja
    QPixmap imagenEstante1("/home/pancho/supermercado_proyecto/estante1.png");
    QPixmap imagenEstante2("/home/pancho/supermercado_proyecto/estante2.png");
    QPixmap imagenEstante3("/home/pancho/supermercado_proyecto/estante3.png");
    QPixmap imagenCaja("/home/pancho/supermercado_proyecto/caja.png");

    if (imagenEstante1.isNull() || imagenCaja.isNull() || imagenEstante2.isNull() || imagenEstante3.isNull()) {
        qDebug() << "Error: No se pudieron cargar las imágenes de los elementos.";
        return -1;
    }

    QPixmap estante1Escalado = imagenEstante1.scaled(203, 272, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap estante2Escalado = imagenEstante2.scaled(181, 244, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap estante3Escalado = imagenEstante3.scaled(181, 244, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap cajaEscalado = imagenCaja.scaled(300, 217, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem *estanteItem1 = new QGraphicsPixmapItem(estante1Escalado);
    QGraphicsPixmapItem *estanteItem2 = new QGraphicsPixmapItem(estante2Escalado);
    QGraphicsPixmapItem *estanteItem3 = new QGraphicsPixmapItem(estante3Escalado);
    QGraphicsPixmapItem *estanteItem4 = new QGraphicsPixmapItem(estante1Escalado);
    QGraphicsPixmapItem *estanteItem5 = new QGraphicsPixmapItem(estante2Escalado);
    QGraphicsPixmapItem *estanteItem6 = new QGraphicsPixmapItem(estante3Escalado);
    QGraphicsPixmapItem *cajaItem = new QGraphicsPixmapItem(cajaEscalado);

    estanteItem1->setPos(100, 10);
    estanteItem2->setPos(350, 13);
    estanteItem3->setPos(570, 13);
    estanteItem4->setPos(100, 300);
    estanteItem5->setPos(350, 303);
    estanteItem6->setPos(570, 303);
    cajaItem->setPos(800, 160);

    escena.addItem(estanteItem1);
    escena.addItem(estanteItem2);
    escena.addItem(estanteItem3);
    escena.addItem(estanteItem4);
    escena.addItem(estanteItem5);
    escena.addItem(estanteItem6);
    escena.addItem(cajaItem);

    QGraphicsView vista(&escena);
    vista.setWindowTitle("Generador de Carritos en Qt");
    vista.resize(1000, 600);

    QPushButton *botonGeneracion = new QPushButton("Iniciar Generación");
    botonGeneracion->setCheckable(true);
    QPushButton *botonFacturas = new QPushButton("Mostrar Facturas");

    QHBoxLayout *layoutBotones = new QHBoxLayout;
    layoutBotones->addWidget(botonGeneracion);
    layoutBotones->addWidget(botonFacturas);

    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(layoutBotones);
    layoutPrincipal->addWidget(&vista);

    QHBoxLayout *layoutFinal = new QHBoxLayout;
    layoutFinal->addLayout(layoutPrincipal);
    layoutFinal->addWidget(widgetClientes); // Usar el widget de clientes

    layoutFinal->setStretch(0, 3);
    layoutFinal->setStretch(1, 1);

    QWidget ventana;
    ventana.setLayout(layoutFinal);
    ventana.setFixedSize(1400, 700);
    ventana.show();

    QObject::connect(botonGeneracion, &QPushButton::toggled, [&](bool checked) {
        if (checked) {
            generador.iniciarGeneracion();
            botonGeneracion->setText("Detener Generación");
        } else {
            generador.detenerGeneracion();
            botonGeneracion->setText("Iniciar Generación");
        }
    });

    QObject::connect(botonFacturas, &QPushButton::clicked, [&]() {
        // Mostrar la última factura generada
        textEditFactura->setText(textEditFactura->toPlainText()); // Esto no es necesario, pero puedes agregar lógica adicional si lo deseas
    });

    QTimer temporizador;
    QObject::connect(&temporizador, &QTimer::timeout, &escena, &QGraphicsScene::advance);
    temporizador.start(10);

    return aplicacion.exec();
}

#include "main.moc"
*/

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QList>
#include <QPointF>
#include <QLineF>
#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextEdit>
#include <QStringList>
#include <algorithm>
#include <random>
#include <QMap>
#include <QSplitter>
#include <QCoreApplication>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QString>
#include <ctime>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <QVector>

// Declaración de variables globales
std::vector<std::string> cedulasgeneradas;
std::vector<std::string> telefonogeneradas;

std::string GenerarNombre() {
    std::vector<std::string> nombres = {"Juan", "María", "Carlos", "Ana", "Luis"};
    std::vector<std::string> apellidos = {"Pérez", "Gómez", "López", "Martínez", "Rodríguez", "Rivas"};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribNombre(0, nombres.size() - 1);
    std::uniform_int_distribution<> distribApellido(0, apellidos.size() - 1);

    std::string nombreCompleto = nombres[distribNombre(gen)] + " " + apellidos[distribApellido(gen)];
    return nombreCompleto;
}

std::string GenerarCedula() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(4000000, 40000000);

    std::string cedula;
    bool Duplicada;

    do {
        int cedulaNum = distrib(gen);
        cedula = std::to_string(cedulaNum);
        Duplicada = (std::find(cedulasgeneradas.begin(), cedulasgeneradas.end(), cedula) != cedulasgeneradas.end());
    } while (Duplicada);

    cedulasgeneradas.push_back(cedula);
    return cedulasgeneradas.back();
}

std::string GenerarTelefono() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib1(500000, 40000000);
    std::uniform_int_distribution<int> distrib2(1, 5);

    int prefijo;
    std::string telefono;
    bool Duplicada;

    do {
        int telefonoNum = distrib1(gen);
        prefijo = distrib2(gen);
        std::stringstream completo;

        switch (prefijo) {
        case 1:
            completo << "0424" << telefonoNum;
            break;
        case 2:
            completo << "0414" << telefonoNum;
            break;
        case 3:
            completo << "0412" << telefonoNum;
            break;
        case 4:
            completo << "0416" << telefonoNum;
            break;
        case 5:
            completo << "0426" << telefonoNum;
            break;
        }

        telefono = completo.str();
        Duplicada = (std::find(telefonogeneradas.begin(), telefonogeneradas.end(), telefono) != telefonogeneradas.end());
    } while (Duplicada);

    telefonogeneradas.push_back(telefono);
    return telefonogeneradas.back();
}

class Cliente {
public:
    Cliente() : id(0), nombre(""), estado("") {}
    Cliente(int id, const QString &nombre, const QString &estado, const QMap<QString, int> &listaCompra = QMap<QString, int>())
        : id(id), nombre(nombre), estado(estado), listaCompra(listaCompra) {}  // Corrección aquí

    int getId() const { return id; }
    QString getNombre() const { return nombre; }
    QString getEstado() const { return estado; }
    QMap<QString, int> getListaCompra() const { return listaCompra; }
    QString getCedula() const { return cedula; }
    QString getTelefono() const { return telefono; }

    void setEstado(const QString &nuevoEstado) { estado = nuevoEstado; }

    void agregarProducto(const QString &producto, int cantidad) {
        if (listaCompra.contains(producto)) {
            if (listaCompra[producto] + cantidad <= 3) {
                listaCompra[producto] += cantidad;
            } else {
                listaCompra[producto] = 3;
            }
        } else {
            if (cantidad <= 3) {
                listaCompra[producto] = cantidad;
            } else {
                listaCompra[producto] = 3;
            }
        }
    }

    void eliminarProducto(const QString &producto) { listaCompra.remove(producto); }

    void generarDatos() {
        nombre = QString::fromStdString(GenerarNombre());
        cedula = QString::fromStdString(GenerarCedula());
        telefono = QString::fromStdString(GenerarTelefono());
        generarListaCompra();
    }

    void generarListaCompra() {
        QStringList productosDisponibles = {"Manzanas", "Pan", "Leche", "Arroz", "Frijoles", "Aceite", "Pollo", "Papas", "Cebolla"};

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> disProductos(1, 5);
        std::uniform_int_distribution<> disCantidad(1, 3);

        int totalProductos = 0;
        int numProductos = disProductos(gen);

        for (int i = 0; i < numProductos; ++i) {
            if (totalProductos >= 30) break;

            QString producto = productosDisponibles[std::uniform_int_distribution<>(0, productosDisponibles.size() - 1)(gen)];
            int cantidad = disCantidad(gen);

            if (totalProductos + cantidad > 30) {
                cantidad = 30 - totalProductos;
            }

            agregarProducto(producto, cantidad);
            totalProductos += cantidad;
        }
    }

private:
    int id;
    QString nombre;
    QString estado;
    QMap<QString, int> listaCompra;
    QString cedula;
    QString telefono;
};

class GestorClientes {
public:
    void agregarCliente(const Cliente &cliente) {
        clientes[cliente.getId()] = cliente;
    }

    void eliminarCliente(int id) {
        clientes.remove(id);
    }

    Cliente obtenerCliente(int id) const {
        return clientes.value(id);
    }

    QList<Cliente> obtenerTodosLosClientes() const {
        return clientes.values();
    }

    bool existeCliente(int id) const {
        return clientes.contains(id);
    }

private:
    QMap<int, Cliente> clientes;
};

class Factura {
public:
    Factura(const Cliente &cliente) : cliente(cliente) {}

    void generarFactura() {
        contenido = "Factura para: " + cliente.getNombre() + "\n";
        contenido += "Cédula: " + cliente.getCedula() + "\n";
        contenido += "Teléfono: " + cliente.getTelefono() + "\n\n";
        contenido += "Productos:\n";

        double total = 0.0;
        QMap<QString, int> listaCompra = cliente.getListaCompra();
        for (auto it = listaCompra.begin(); it != listaCompra.end(); ++it) {
            double precio = obtenerPrecioProducto(it.key());
            double subtotal = precio * it.value();
            contenido += it.key() + " x " + QString::number(it.value()) + " = $" + QString::number(subtotal, 'f', 2) + "\n";
            total += subtotal;
        }

        contenido += "\nTotal: $" + QString::number(total, 'f', 2) + "\n";
    }

    void guardarFactura(const QString &rutaArchivo) {
        QFile archivo(rutaArchivo);
        if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&archivo);
            out << contenido;
            archivo.close();
        } else {
            qDebug() << "Error: No se pudo guardar la factura.";
        }
    }

    QString getContenido() const { return contenido; }

private:
    Cliente cliente;
    QString contenido;

    double obtenerPrecioProducto(const QString &producto) {
        QMap<QString, double> precios = {
            {"Manzanas", 1.50},
            {"Pan", 0.80},
            {"Leche", 1.20},
            {"Arroz", 2.00},
            {"Frijoles", 1.80},
            {"Aceite", 3.50},
            {"Pollo", 5.00},
            {"Papas", 1.00},
            {"Cebolla", 0.50}
        };
        return precios.value(producto, 0.0);
    }
};

class DialogoCliente : public QDialog {
public:
    DialogoCliente(const Cliente &cliente, QWidget *padre = nullptr)
        : QDialog(padre) {
        setWindowTitle("Detalles del Cliente");
        setFixedSize(400, 300);

        QLabel *labelId = new QLabel("ID: " + QString::number(cliente.getId()), this);
        QLabel *labelNombre = new QLabel("Nombre: " + cliente.getNombre(), this);
        QLabel *labelEstado = new QLabel("Estado: " + cliente.getEstado(), this);
        QLabel *labelCedula = new QLabel("Cédula: " + cliente.getCedula(), this);
        QLabel *labelTelefono = new QLabel("Teléfono: " + cliente.getTelefono(), this);

        QTextEdit *textEditListaCompra = new QTextEdit(this);
        textEditListaCompra->setReadOnly(true);

        QString listaCompraTexto = "Lista de la compra:\n";
        QMap<QString, int> listaCompra = cliente.getListaCompra();
        for (auto it = listaCompra.begin(); it != listaCompra.end(); ++it) {
            listaCompraTexto += it.key() + ": " + QString::number(it.value()) + "\n";
        }

        textEditListaCompra->setText(listaCompraTexto);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(labelId);
        layout->addWidget(labelNombre);
        layout->addWidget(labelEstado);
        layout->addWidget(labelCedula);
        layout->addWidget(labelTelefono);
        layout->addWidget(textEditListaCompra);

        setLayout(layout);
    }
};

class VentanaFactura : public QDialog {
public:
    VentanaFactura(const QString &rutaArchivo, QWidget *padre = nullptr) : QDialog(padre) {
        setWindowTitle ("Detalles de la Factura");
        setFixedSize(400, 600);

        QTextEdit *editorTexto = new QTextEdit(this);
        editorTexto->setReadOnly(true);

        QFile archivo(rutaArchivo);
        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&archivo);
            editorTexto->setText(in.readAll());
            archivo.close();
        } else {
            editorTexto->setText("Error: No se pudo abrir el archivo.");
        }

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(editorTexto);
        setLayout(layout);
    }
};

class CarritoMovil : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    CarritoMovil(const QPixmap &pixmap, Cliente *cliente, QGraphicsItem *padre = nullptr)
        : QObject(nullptr),
        QGraphicsPixmapItem(pixmap, padre),
        indiceActual(0),
        velocidad(1),
        cliente(cliente) {
        textoId = new QGraphicsTextItem(QString::number(cliente->getId()), this);
        textoId->setDefaultTextColor(Qt::black);
        textoId->setFont(QFont("Arial", 12));

        QRectF rectCarrito = boundingRect();
        textoId->setPos(rectCarrito.width() / 2 - textoId->boundingRect().width() / 2, -20);
    }

    void establecerRuta(const QList<QPointF>& nuevaRuta) {
        ruta = nuevaRuta;
    }

    void advance(int fase) override {
        if (fase && !ruta.isEmpty()) {
            QPointF objetivo = ruta[indiceActual];
            QPointF posicionActual = pos();

            QPointF direccion = objetivo - posicionActual;
            qreal distancia = QLineF(posicionActual, objetivo).length();

            if (distancia > 1) {
                direccion /= distancia;
                setPos(posicionActual + direccion * velocidad);
            } else {
                indiceActual++;
                if (indiceActual >= ruta.size()) {
                    emit carritoCompletado(cliente->getId(), "/home/pancho/supermercado_proyecto/factura_ultimo_cliente.txt");
                    if (scene()) scene()->removeItem(this);
                    deleteLater();
                }
            }
        }
    }

    Cliente* getCliente() const { return cliente; }

signals:
    void carritoCompletado(int clienteId, const QString &rutaArchivo);

private:
    QList<QPointF> ruta;
    int indiceActual;
    qreal velocidad;
    Cliente *cliente;
    QGraphicsTextItem *textoId;
};

class GeneradorCarritos : public QObject {
    Q_OBJECT

public:
    GeneradorCarritos(QGraphicsScene *escena, GestorClientes *gestorClientes, QHBoxLayout *layoutClientes, QTextEdit *textEditFactura, QObject *padre = nullptr)
        : QObject(padre), escena(escena), gestorClientes(gestorClientes), layoutClientes(layoutClientes), textEditFactura(textEditFactura), generando(false) {
        ruta = {
            QPointF(10, 10),
            QPointF(10, 275),
            QPointF(50, 275),
            QPointF(50, 90),
            QPointF(250, 90),
            QPointF(250, 380),
            QPointF(450, 380),
            QPointF(450, 90),
            QPointF(650, 90),
            QPointF(650, 300),
            QPointF(935, 300)
        };

        temporizador = new QTimer(this);
        connect(temporizador, &QTimer::timeout, this, &GeneradorCarritos::generarCarrito);
    }

    void iniciarGeneracion() {
        if (!generando) {
            temporizador->start(10000);
            generando = true;
        }
    }

    void detenerGeneracion() {
        if (generando) {
            temporizador->stop();
            generando = false;
        }
    }

private slots:
    void generarCarrito() {
        QPixmap carritoPixmap("/home/pancho/supermercado_proyecto/carrito.png");
        if (carritoPixmap.isNull()) {
            qDebug() << "Error: No se pudo cargar la imagen del carrito.";
            return;
        }

        QPixmap carritoEscalado = carritoPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        int nuevoId = gestorClientes->obtenerTodosLosClientes().isEmpty() ? 1 : gestorClientes->obtenerTodosLosClientes().last().getId() + 1;
        Cliente cliente(nuevoId, "", "Comprando");
        cliente.generarDatos();

        gestorClientes->agregarCliente(cliente);

        actualizarInformacionClientes(layoutClientes, gestorClientes->obtenerTodosLosClientes());

        CarritoMovil *carrito = new CarritoMovil(carritoEscalado, new Cliente(cliente));
        escena->addItem(carrito);
        carrito->establecerRuta(ruta);

        connect(carrito, &CarritoMovil::carritoCompletado, this, &GeneradorCarritos::manejarCarritoCompletado);
    }

    void manejarCarritoCompletado(int clienteId, const QString &rutaArchivo) {
        QFile archivoFactura(rutaArchivo);  // Asignar la ruta del archivo
        if (archivoFactura.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&archivoFactura);

            // Generar la factura
            Cliente cliente = gestorClientes->obtenerCliente(clienteId);
            Factura factura(cliente);
            factura.generarFactura();

            // Escribir la factura en el archivo
            out << factura.getContenido();
            archivoFactura.close();

            // Mostrar la factura en el QTextEdit
            textEditFactura->setText(factura.getContenido());
        } else {
            qDebug() << "Error: No se pudo abrir el archivo para escribir.";
        }

        // Eliminar el cliente del gestor
        gestorClientes->eliminarCliente(clienteId);
        actualizarInformacionClientes(layoutClientes, gestorClientes->obtenerTodosLosClientes());
    }

private:
    QGraphicsScene *escena;
    QList<QPointF> ruta;
    QTimer *temporizador;
    bool generando;
    GestorClientes *gestorClientes;
    QHBoxLayout *layoutClientes;
    QTextEdit *textEditFactura;

    void actualizarInformacionClientes(QHBoxLayout *layoutClientes, const QList<Cliente> &clientes) {
        QLayoutItem *item;
        while ((item = layoutClientes->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        for (const Cliente &cliente : clientes) {
            QWidget *widgetCliente = new QWidget;
            QVBoxLayout *layoutCliente = new QVBoxLayout(widgetCliente);

            QString texto = QString("ID: %1\nNombre: %2\nEstado: %3")
                                .arg(cliente.getId())
                                .arg(cliente.getNombre())
                                .arg(cliente.getEstado());

            QLabel *labelCliente = new QLabel(texto);
            layoutCliente->addWidget(labelCliente);

            QPushButton *botonDetalles = new QPushButton("Ver detalles");
            QObject::connect(botonDetalles, &QPushButton::clicked, [cliente]() {
                DialogoCliente *dialogo = new DialogoCliente(cliente);
                dialogo->exec();
            });
            layoutCliente->addWidget(botonDetalles);

            layoutClientes->addWidget(widgetCliente);
        }
    }
};

void cargarArchivoTxt(QTextEdit *textEdit, const QString &rutaArchivo) {
    QFile archivo(rutaArchivo);
    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&archivo);
        textEdit->setText(in.readAll());
        archivo.close();
    } else {
        textEdit->setText("Error: No se pudo abrir el archivo.");
    }
}

int main(int argc, char *argv[]) {
    QApplication aplicacion(argc, argv);

    QGraphicsScene escena;
    escena.setSceneRect(0, 0, 1000, 600);

    GestorClientes gestorClientes;

    QWidget *widgetClientes = new QWidget;
    QHBoxLayout *layoutClientes = new QHBoxLayout(widgetClientes);
    widgetClientes->setMinimumWidth(400);

    QTextEdit *textEditFactura = new QTextEdit;
    textEditFactura->setReadOnly(true);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(widgetClientes);

    QWidget *widgetArchivo = new QWidget;
    QVBoxLayout *layoutArchivo = new QVBoxLayout(widgetArchivo);
    layoutArchivo->addWidget(textEditFactura);
    splitter->addWidget(widgetArchivo);

    GeneradorCarritos generador(&escena, &gestorClientes, layoutClientes, textEditFactura);

    QPixmap imagenFondo("/home/pancho/supermercado_proyecto/suelo1.png");
    if (imagenFondo.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen de fondo.";
        return -1;
    }

    QPixmap fondoEscalado = imagenFondo.scaled(306, 204, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QBrush fondo(fondoEscalado);
    fondo.setStyle(Qt::TexturePattern);
    escena.setBackgroundBrush(fondo);

    QPixmap imagenEstante1("/home/pancho/supermercado_proyecto/estante1.png");
    QPixmap imagenEstante2("/home/pancho/supermercado_proyecto/estante2.png");
    QPixmap imagenEstante3("/home/pancho/supermercado_proyecto/estante3.png");
    QPixmap imagenCaja("/home/pancho/supermercado_proyecto/caja.png");

    if (imagenEstante1.isNull() || imagenCaja.isNull() || imagenEstante2.isNull() || imagenEstante3.isNull()) {
        qDebug() << "Error: No se pudieron cargar las imágenes de los elementos.";
        return -1;
    }

    QPixmap estante1Escalado = imagenEstante1.scaled(203, 272, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap estante2Escalado = imagenEstante2.scaled(181, 244, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap estante3Escalado = imagenEstante3.scaled(181, 244, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap cajaEscalado = imagenCaja.scaled(300, 217, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem *estanteItem1 = new QGraphicsPixmapItem(estante1Escalado);
    QGraphicsPixmapItem *estanteItem2 = new QGraphicsPixmapItem(estante2Escalado);
    QGraphicsPixmapItem *estanteItem3 = new QGraphicsPixmapItem(estante3Escalado);
    QGraphicsPixmapItem *estanteItem4 = new QGraphicsPixmapItem(estante1Escalado);
    QGraphicsPixmapItem *estanteItem5 = new QGraphicsPixmapItem(estante2Escalado);
    QGraphicsPixmapItem *estanteItem6 = new QGraphicsPixmapItem(estante3Escalado);
    QGraphicsPixmapItem *cajaItem = new QGraphicsPixmapItem(cajaEscalado);

    estanteItem1->setPos(100, 10);
    estanteItem2->setPos(350, 13);
    estanteItem3->setPos(570, 13);
    estanteItem4->setPos(100, 300);
    estanteItem5->setPos(350, 303);
    estanteItem6->setPos(570, 303);
    cajaItem->setPos(800, 160);

    escena.addItem(estanteItem1);
    escena.addItem(estanteItem2);
    escena.addItem(estanteItem3);
    escena.addItem(estanteItem4);
    escena.addItem(estanteItem5);
    escena.addItem(estanteItem6);
    escena.addItem(cajaItem);

    QGraphicsView vista(&escena);
    vista.setWindowTitle("Generador de Carritos en Qt");
    vista.resize(1000, 600);

    QPushButton *botonGeneracion = new QPushButton("Iniciar Generación");
    botonGeneracion->setCheckable(true);
    QPushButton *botonFacturas = new QPushButton("Mostrar Facturas");

    QHBoxLayout *layoutBotones = new QHBoxLayout;
    layoutBotones->addWidget(botonGeneracion);
    layoutBotones->addWidget(botonFacturas);

    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(layoutBotones);
    layoutPrincipal->addWidget(&vista);

    QHBoxLayout *layoutFinal = new QHBoxLayout;
    layoutFinal->addLayout(layoutPrincipal);
    layoutFinal->addWidget(splitter);

    layoutFinal->setStretch(0, 3);
    layoutFinal->setStretch(1, 1);

    QWidget ventana;
    ventana.setLayout(layoutFinal);
    ventana.setFixedSize(1400, 700);
    ventana.show();

    QObject::connect(botonGeneracion, &QPushButton::toggled, [&](bool checked) {
        if (checked) {
            generador.iniciarGeneracion();
            botonGeneracion->setText("Detener Generación");
        } else {
            generador.detenerGeneracion();
            botonGeneracion->setText("Iniciar Generación");
        }
    });

    QObject::connect(botonFacturas, &QPushButton::clicked, [&]() {
        VentanaFactura *ventanaFactura = new VentanaFactura("/home/pancho/supermercado_proyecto/factura_ultimo_cliente.txt");
        ventanaFactura->exec();
    });

    QTimer temporizador;
    QObject::connect(&temporizador, &QTimer::timeout, &escena, &QGraphicsScene::advance);
    temporizador.start(10);

    return aplicacion.exec();
}

#include "main.moc"
