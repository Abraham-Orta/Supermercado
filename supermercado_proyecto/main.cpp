#include <QApplication>         // Incluye la clase QApplication para gestionar la aplicación GUI.
#include <QGraphicsScene>       // Incluye la clase QGraphicsScene para gestionar una escena con elementos gráficos.
#include <QGraphicsView>        // Incluye la clase QGraphicsView para mostrar la QGraphicsScene.
#include <QGraphicsPixmapItem>  // Incluye la clase QGraphicsPixmapItem para mostrar imágenes en la escena.
#include <QTimer>               // Incluye la clase QTimer para crear temporizadores que desencadenan eventos.
#include <QList>                // Incluye la clase QList para utilizar una estructura de datos de lista.
#include <QPointF>              // Incluye la clase QPointF para representar puntos en 2D.
#include <QLineF>               // Incluye la clase QLineF para representar líneas en el espacio 2D.
#include <QDebug>               // Incluye la clase QDebug para salida de depuración.
#include <QWidget>              // Incluye la clase QWidget para crear widgets GUI.
#include <QPushButton>          // Incluye la clase QPushButton para crear botones.
#include <QVBoxLayout>          // Incluye la clase QVBoxLayout para gestionar el diseño vertical.
#include <QDialog>              // Incluye la clase QDialog para crear ventanas de diálogo.
#include <QLabel>               // Incluye la clase QLabel para mostrar texto o imágenes.
#include <QFile>                // Incluye la clase QFile para manejo de archivos.
#include <QTextStream>          // Incluye la clase QTextStream para leer y escribir archivos de texto.
#include <QMessageBox>          // Incluye la clase QMessageBox para mostrar cuadros de mensaje.
#include <QTextEdit>            // Incluye la clase QTextEdit para mostrar y editar texto.

class CarritoMovil : public QGraphicsPixmapItem { // Define la clase CarritoMovil heredando de QGraphicsPixmapItem.
public:
    CarritoMovil(const QPixmap &pixmap, QGraphicsItem *padre = nullptr) // Constructor para CarritoMovil.
        : QGraphicsPixmapItem(pixmap, padre), indiceActual(0), velocidad(1) {} // Inicializa la clase base y las variables miembro.

    void establecerRuta(const QList<QPointF>& nuevaRuta) { // Método para establecer la ruta del carrito.
        ruta = nuevaRuta; // Asigna la nueva ruta a la variable miembro.
    }

    void advance(int fase) override { // Sobrescribe el método advance para actualizar la posición del carrito.
        if (fase && !ruta.isEmpty()) { // Verifica si la fase es válida y la ruta no está vacía.
            QPointF objetivo = ruta[indiceActual]; // Obtiene el punto objetivo actual de la ruta.
            QPointF posicionActual = pos(); // Obtiene la posición actual del carrito.

            QPointF direccion = objetivo - posicionActual; // Calcula el vector de dirección hacia el objetivo.
            qreal distancia = QLineF(posicionActual, objetivo).length(); // Calcula la distancia al objetivo.

            if (distancia > 1) { // Si la distancia es mayor a 1 píxel.
                direccion /= distancia; // Normaliza el vector de dirección.
                setPos(posicionActual + direccion * velocidad); // Mueve el carrito hacia el objetivo.
            } else { // Si el carrito ha alcanzado el objetivo.
                indiceActual++; // Avanza al siguiente punto en la ruta.
                if (indiceActual >= ruta.size()) { // Si se ha alcanzado el final de la ruta.
                    scene()->removeItem(this); // Elimina el carrito de la escena.
                    delete this; // Libera la memoria asignada para el carrito.
                }
            }
        }
    }

private:
    QList<QPointF> ruta; // Lista para almacenar los puntos de la ruta.
    int indiceActual; // Índice actual en la ruta.
    qreal velocidad; // Velocidad del carrito.
};

class GeneradorCarritos : public QObject { // Define la clase GeneradorCarritos heredando de QObject.
    Q_OBJECT // Macro para habilitar señales y slots en Qt.
public:
    GeneradorCarritos(QGraphicsScene *escena, QObject *padre = nullptr) // Constructor para GeneradorCarritos.
        : QObject(padre), escena(escena), generando(false) { // Inicializa las variables miembro.
        ruta = { // Define la ruta que seguirán los carritos.
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
            QPointF(935, 300) // Punto final para completar la ruta.
        };

        temporizador = new QTimer(this); // Crea un nuevo temporizador.
        connect(temporizador, &QTimer::timeout, this, &GeneradorCarritos::generarCarrito); // Conecta la señal de timeout del temporizador al slot generarCarrito.
    }

    void iniciarGeneracion() { // Método para iniciar la generación de carritos.
        if (!generando) { // Verifica si no se están generando carritos ya.
            temporizador->start(1000); // Inicia el temporizador para generar un nuevo carrito cada segundo.
            generando = true; // Establece la bandera de generación a verdadero.
        }
    }

    void detenerGeneracion() { // Método para detener la generación de carritos.
        if (generando) { // Verifica si se están generando carritos.
            temporizador->stop(); // Detiene el temporizador.
            generando = false; // Establece la bandera de generación a falso.
        }
    }

private slots:
    void generarCarrito() { // Slot para generar un nuevo carrito.
        QPixmap carritoPixmap("/home/pancho/supermercado_proyecto/carrito.png"); // Carga la imagen del carrito.
        if (carritoPixmap.isNull()) { // Verifica si la imagen no se cargó correctamente.
            qDebug() << "Error: No se pudo cargar la imagen del carrito."; // Muestra un mensaje de error.
            return; // Sale de la función.
        }

        QPixmap carritoEscalado = carritoPixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Escala la imagen del carrito.

        CarritoMovil *carrito = new CarritoMovil(carritoEscalado); // Crea un nuevo objeto CarritoMovil.
        escena->addItem(carrito); // Agrega el carrito a la escena.
        carrito->establecerRuta(ruta); // Establece la ruta para el carrito.
    }

private:
    QGraphicsScene *escena; // Puntero a la escena gráfica.
    QList<QPointF> ruta; // Lista para almacenar los puntos de la ruta.
    QTimer *temporizador; // Puntero al temporizador para generar carritos.
    bool generando; // Bandera para indicar si se están generando carritos.
};

class VentanaFactura : public QDialog { // Define la clase VentanaFactura heredando de QDialog.
public:
    VentanaFactura(const QString &rutaArchivo, QWidget *padre = nullptr) : QDialog(padre) { // Constructor para VentanaFactura.
        setWindowTitle("Detalles de la Factura"); // Establece el título de la ventana.
        setFixedSize(400, 300); // Establece un tamaño fijo para el diálogo.

        QTextEdit *editorTexto = new QTextEdit(this); // Crea un QTextEdit para mostrar el contenido de la factura.
        editorTexto->setReadOnly(true); // Hace que el QTextEdit sea de solo lectura.

        QFile archivo(rutaArchivo); // Crea un objeto QFile para el archivo de factura especificado.
        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) { // Intenta abrir el archivo para lectura.
            QTextStream in(&archivo); // Crea un QTextStream para leer el archivo.
            editorTexto->setText(in.readAll()); // Lee todo el archivo y lo establece como texto del editor.
            archivo.close(); // Cierra el archivo.
        } else {
            editorTexto->setText("Error: No se pudo abrir el archivo."); // Establece un mensaje de error si no se pudo abrir el archivo.
        }

        QVBoxLayout *layout = new QVBoxLayout(this); // Crea un diseño vertical para el diálogo.
        layout->addWidget(editorTexto); // Agrega el QTextEdit al diseño.
        setLayout(layout); // Establece el diseño para el diálogo.
    }
};

int main(int argc, char *argv[]) { // Función principal de la aplicación.
    QApplication aplicacion(argc, argv); // Crea el objeto QApplication.

    QGraphicsScene escena; // Crea un objeto QGraphicsScene.
    escena.setSceneRect(0, 0, 1000, 600); // Establece las dimensiones del rectángulo de la escena.

    GeneradorCarritos generador(&escena); // Crea un objeto GeneradorCarritos con la escena.

    QPixmap imagenFondo("/home/pancho/supermercado_proyecto/suelo1.png"); // Carga la imagen de fondo.
    if (imagenFondo.isNull()) { // Verifica si la imagen no se cargó correctamente.
        qDebug() << "Error: No se pudo cargar la imagen de fondo."; // Muestra un mensaje de error.
        return -1; // Sale de la aplicación con un código de error.
    }

    QPixmap fondoEscalado = imagenFondo.scaled(306, 204, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Escala la imagen de fondo.

    QBrush fondo(fondoEscalado); // Crea un QBrush con la imagen de fondo escalada.
    fondo.setStyle(Qt::TexturePattern); // Establece el estilo del pincel a patrón de textura.

    escena.setBackgroundBrush(fondo); // Establece el pincel de fondo para la escena.

    QPixmap imagenEstante("/home/pancho/supermercado_proyecto/estante.png"); // Carga la imagen del estante.
    if (imagenEstante.isNull()) { // Verifica si la imagen no se cargó correctamente.
        qDebug() << "Error: No se pudo cargar la imagen del estante."; // Muestra un mensaje de error.
        return -1; // Sale de la aplicación con un código de error.
    }

    QPixmap imagenCaja("/home/pancho/supermercado_proyecto/caja.png"); // Carga la imagen de la caja.
    if (imagenCaja.isNull()) { // Verifica si la imagen no se cargó correctamente.
        qDebug() << "Error: No se pudo cargar la imagen de la caja."; // Muestra un mensaje de error.
        return -1; // Sale de la aplicación con un código de error.
    }

    QPixmap estanteEscalado = imagenEstante.scaled(203, 272, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Escala la imagen del estante.
    QPixmap cajaEscalado = imagenCaja.scaled(300, 217, Qt::KeepAspectRatio, Qt::SmoothTransformation); // Escala la imagen de la caja.

    QGraphicsPixmapItem *estanteItem1 = new QGraphicsPixmapItem(estanteEscalado); // Crea un QGraphicsPixmapItem para el primer estante.
    QGraphicsPixmapItem *estanteItem2 = new QGraphicsPixmapItem(estanteEscalado); // Crea un QGraphicsPixmapItem para el segundo estante.
    QGraphicsPixmapItem *estanteItem3 = new QGraphicsPixmapItem(estanteEscalado); // Crea un QGraphicsPixmapItem para el tercer estante.

    QGraphicsPixmapItem *cajaItem = new QGraphicsPixmapItem(cajaEscalado); // Crea un QGraphicsPixmapItem para la caja.

    estanteItem1->setPos(100, 165); // Establece la posición del primer estante.
    estanteItem2->setPos(300, 165); // Establece la posición del segundo estante.
    estanteItem3->setPos(500, 165); // Establece la posición del tercer estante.
    cajaItem->setPos(800, 190); // Establece la posición de la caja.

    escena.addItem(estanteItem1); // Agrega el primer estante a la escena.
    escena.addItem(estanteItem2); // Agrega el segundo estante a la escena.
    escena.addItem(estanteItem3); // Agrega el tercer estante a la escena.
    escena.addItem(cajaItem); // Agrega la caja a la escena.

    QGraphicsView vista(&escena); // Crea un QGraphicsView para mostrar la escena.
    vista.setWindowTitle("Generador de Carritos en Qt"); // Establece el título de la ventana.
    vista.resize(1000, 600); // Redimensiona la vista.
    vista.show(); // Muestra la vista.

    QPushButton *botonGeneracion = new QPushButton("Iniciar Generación"); // Crea un botón para iniciar la generación de carritos.
    botonGeneracion->setCheckable(true); // Hace que el botón sea seleccionable.

    QObject::connect(botonGeneracion, &QPushButton::toggled, [&generador, botonGeneracion](bool checked) { // Conecta la señal de toggled del botón a una función lambda.
        if (checked) { // Si el botón está seleccionado.
            generador.iniciarGeneracion(); // Inicia la generación de carritos.
            botonGeneracion->setText("Detener Generación"); // Cambia el texto del botón.
        } else { // Si el botón no está seleccionado.
            generador.detenerGeneracion(); // Detiene la generación de carritos.
            botonGeneracion->setText("Iniciar Generación"); // Cambia el texto del botón de vuelta.
        }
    });

    QPushButton *botonFacturas = new QPushButton("Mostrar Facturas"); // Crea un botón para mostrar facturas.

    QObject::connect(botonFacturas, &QPushButton::clicked, [&]() { // Conecta la señal de clicked del botón a una función lambda.
        QString rutaArchivo = "/home/pancho/supermercado_proyecto/factura.txt"; // Especifica la ruta al archivo de factura.

        VentanaFactura *ventana = new VentanaFactura(rutaArchivo); // Crea un objeto VentanaFactura para mostrar la factura.
        ventana->exec(); // Muestra el diálogo de la factura como una ventana modal.
    });

    QVBoxLayout *layout = new QVBoxLayout; // Crea un diseño vertical para organizar la vista y los botones.
    layout->addWidget(&vista); // Agrega la vista gráfica al diseño.
    layout->addWidget(botonGeneracion); // Agrega el botón de generación al diseño.
    layout->addWidget(botonFacturas); // Agrega el botón de facturas al diseño.

    QWidget ventana; // Crea un widget principal para la aplicación.
    ventana.setLayout(layout); // Establece el diseño para el widget principal.
    ventana.setFixedSize(1100, 700); // Establece un tamaño fijo para el widget principal.
    ventana.show(); // Muestra el widget principal.

    QTimer temporizador; // Crea un temporizador para animar los carritos.
    QObject::connect(&temporizador, &QTimer::timeout, &escena, &QGraphicsScene::advance); // Conecta la señal de timeout del temporizador para avanzar la escena.
    temporizador.start(10); // Inicia el temporizador para actualizar la escena cada 10 milisegundos.

    return aplicacion.exec(); // Ejecuta el bucle de eventos de la aplicación.
}

#include "main.moc" // Incluye el archivo generado por MOC para la funcionalidad de señales y slots.
