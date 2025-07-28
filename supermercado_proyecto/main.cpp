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
