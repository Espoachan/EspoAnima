#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QScreen *screen  = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    this->resize(screenWidth,screenHeight);
    this->setMinimumSize(400,300);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    Label = new QLabel("", this);
    Button = new QPushButton("Botón de prueba", this);

    Button->setStyleSheet(
        "background-color:#000; color: white; border-radius: 0px; "

    );

    QIcon Icon("Brush");

    Button->setIcon(Icon);
    connect(Button, &QPushButton::clicked, this, &MainWindow::cuandoPresionoBoton);

}

MainWindow::~MainWindow()
{
}

void MainWindow::cuandoPresionoBoton(){
    Label->setText("Hola mundo");
}


void MainWindow::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);

    int w = this->width();
    int h = this->height();

    Button->setGeometry(w / 4, h / 2, w / 2, 40);
    Label->setGeometry(w / 4, h / 2 -50, w / 2, 30);

}
