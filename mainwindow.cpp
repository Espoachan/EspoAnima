#include "mainwindow.h"
#include "menubarhelper.h"
#include <QDockWidget>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "Canvas.h"
#include <QSlider>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //creamos la pantalla
    QScreen *screen  = QGuiApplication::primaryScreen();
    //guardamos el tamaño de la pantalla en "screenGeomtry"
    QRect screenGeometry = screen->geometry();

    //sacamos de "screenGeomtry" el alto y ancho, y lo guardamos en "screenWidth y "screenHeight" respectivamente
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    //cambiamos el tamaño de la pantalla
    this->resize(screenWidth,screenHeight);
    //seteamos un tamaño minimo
    this->setMinimumSize(400,300);


    //creamos un central widget para canvas
    canvas = new Canvas(this);
        //centralWidget = new QWidget(this);

    //seteamos canvas como centralWidget
    setCentralWidget(canvas);

    //creamos un dock que se llame "tools"
    toolDock = new QDockWidget("Tools", this);
    //seteamos las areas permitidas para el dock (izquierda y derecha)
    toolDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    this->setStyleSheet("background-color: #222;  color: white;");
    int toolDockHeight = toolDock->height();
    toolDock->resize(300, toolDockHeight);
    toolDock->setMinimumWidth(100);

    //hacemos un QWidget para el contenido
    QWidget *dockContent = new QWidget();

    //creamos un QPushButton llamado penBtn, para seleccionar la pluma
    penBtn = new QPushButton("Pen", dockContent);
    //cambiamos su tamaño y posicion
    penBtn->setGeometry(10,50,100,30);

    //creamos un QPushButton llamado eraserBtn, para seleccionar el borrador
    eraserBtn = new QPushButton("Eraser", dockContent);
    //cambiamos su tamaño y posicion, igual que para penBtn
    eraserBtn->setGeometry(10,0,100,30);

    //creamos un boton para seleccionar el color del lapiz
    colorBtn = new QPushButton("Color", dockContent);
    //cambiamos su tamaño y posicion
    colorBtn->setGeometry(10, 100, 100,30);

    //creamos un boton para abrir la ventana de cambiar el tamaño del lápiz
    penSizeBtn = new QPushButton("Pen Size", dockContent);
    //cambiamos su tamaño y posicion
    penSizeBtn->setGeometry(10, 150, 100, 30);

    //conectar penSizeBtn
    connect(penSizeBtn, &QPushButton::clicked, this, [this](){
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Select Pen Size");
        dialog->setFixedSize(250, 120);

        QWidget *contentWidget = new QWidget(dialog);
        QVBoxLayout *layout = new QVBoxLayout(contentWidget);

        QLabel *label = new QLabel(QString("Pen size: %1").arg(canvas->penWidth));
        QSlider *slider = new QSlider(Qt::Horizontal, contentWidget);
        slider->setMinimum(1);
        slider->setMaximum(50);
        slider->setValue(canvas->penWidth);

        QPushButton *acceptBtn = new QPushButton("Accept", dialog);
        acceptBtn->setFixedHeight(30);

        layout->addWidget(label);
        layout->addWidget(slider);
        layout->addSpacing(10);
        layout->addWidget(acceptBtn, 0, Qt::AlignCenter);

        connect(slider, &QSlider::valueChanged, this, [=](int value){
            label->setText(QString("Pen size: 1%").arg(value));
        });

        connect(acceptBtn, &QPushButton::clicked, this, [=](){
            canvas->penWidth = slider->value();
            dialog->accept();

        });

        contentWidget->setLayout(layout);
        QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
        mainLayout->addWidget(contentWidget);
        dialog->setLayout(mainLayout);

        dialog->exec();
    });

    //conectar el boton de color para conseguir el color escogido, y aplicarcelo al lápiz
    connect(colorBtn, &QPushButton::clicked, this, [=](){QColor color = QColorDialog::getColor(canvas->penColor, this, "Select pen color");if(color.isValid()){canvas->setPenColor(color);}});

    //le seteamos el widget "dockContent" a toolDock
    toolDock->setWidget(dockContent);
    //Agregamos el widget, en la parte izquierda a toolDock
    addDockWidget(Qt::LeftDockWidgetArea, toolDock);

    //agregamos la barra de menu superior
    setMenuBar(menuBarHelper::createMenuBar(this));

    //conectamos los botones para cambiar de herramienta
    connect(penBtn, &QPushButton::clicked, [this](){
        canvas->setTool(Canvas::Pen);
        updateToolButtons(Canvas:: Pen);
    });

    connect(eraserBtn, &QPushButton::clicked, [this](){
        canvas->setTool(Canvas::Eraser);
        updateToolButtons(Canvas:: Eraser);
    });


    QString btnStyle= "background-color: #444; border-radius: 2px";
    penBtn->setStyleSheet(btnStyle);
    eraserBtn->setStyleSheet(btnStyle);
    colorBtn->setStyleSheet(btnStyle);
    penSizeBtn->setStyleSheet(btnStyle);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createNewFile(){
    auto answer = QMessageBox::question(this, "New File", "Are you sure you want to create a new file? You are gonna lose anything you have not saved");

    if(answer == QMessageBox::Yes){
        canvas->clearCanvas();
    }else{
        return;
    }
}

void MainWindow::about(){
    QMessageBox::information(this, "About", "EspoAnima is an animation and draw software!!");
}

void MainWindow::resizeEvent(QResizeEvent *event){

    int w = this->width();
    int h = this->height();

    Q_UNUSED(event);
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void MainWindow::openToolsDock(){
    addDockWidget(Qt::LeftDockWidgetArea, toolDock);
    toolDock->show();
}

void MainWindow::exportFn(){
    QString fileName = QFileDialog::getSaveFileName(this, "Export image",  "", "PNG (*.png)");
    if (!fileName.isEmpty()){
        if(!fileName.endsWith(".png", Qt::CaseInsensitive)){
            fileName += ".png";
        }
        if(!canvas->image.save(fileName, "PNG")){
            QMessageBox::warning(this, "Error", "We couldn't export this image for you, sorry. :(");
        }
        else {
            QMessageBox::information(this, "Everything was fine", "Image was exported. :)");
        }
    }
}

void MainWindow::updateToolButtons(Canvas::Tool tool){{
    //seteamos un estilo a los botones
       penBtn->setStyleSheet("background-color: #444; border-radius: 2px");
       eraserBtn->setStyleSheet("background-color: #444; border-radius: 2px");
       QString activeStyle = "background-color: #555; border-radius: 2px";

       if(tool == Canvas::Pen){
           penBtn->setStyleSheet(activeStyle);
       }else if (tool == Canvas::Eraser){
           eraserBtn->setStyleSheet(activeStyle);
       }
    }
}
