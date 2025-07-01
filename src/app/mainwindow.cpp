#include "headers/mainwindow.h"
#include "headers/menubarhelper.h"
#include "colorpicker/ColorPicker.h"
#include "headers/NewProjectWindow.h"
#include "headers/NewProjectDialog.h"
#include "headers/Preferences.h"
#include "headers/globals.h"
#include "headers/Canvas.h"

#include <QDockWidget>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolButton>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QShortcut>
#include <QInputDialog>
#include <QGraphicsDropShadowEffect>

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
    this->resize(screenWidth, screenHeight);
    //seteamos un tamaño minimo
    this->setMinimumSize(screenWidth, screenHeight);
    this->setMaximumSize(screenWidth - 1, screenHeight - 1);

    preferences = new Preferences(this);

    newProjectWindow = new NewProjectWindow(this);

    welcomeNewFile();

    //creamos un central widget para canvas
    canvas = new Canvas(this);
    //centralWidget = new QWidget(this);
    if (canvas->layerCount() == 0) {
        canvas->layerCount() + 1;
        canvas->addLayer(1280, 720, Qt::white);
    }

    layerPanel = new LayerPanel(canvas, this);
    layerPanel->setWindowTitle("Layers");
    layerPanel->setMinimumWidth(150);
    layerPanel->setMaximumWidth(300);
    addDockWidget(Qt::RightDockWidgetArea, layerPanel);

    colorPicker = new ColorPicker(this, canvas);
    colorPickerDock = new QDockWidget("Color Picker", this);
    colorPickerDock->setMinimumSize(300, 300);
    colorPickerDock->setMaximumSize(300, 300);
    colorPickerDock->setWidget(colorPicker);
    addDockWidget(Qt::RightDockWidgetArea, colorPickerDock);

    int defaultWidth = 1280;
    int defaultHeight = 720;
    QColor defaultBgColor = Qt::white;


    //timeline = new FrameTimeLine(this);
    canvas->initializeNewCanvas(defaultWidth, defaultHeight, defaultBgColor);

    //seteamos canvas como centralWidget
    setCentralWidget(canvas);

    timeline = new FrameTimeLine(this);
    addDockWidget(Qt::BottomDockWidgetArea, timeline);
    timeline->addNewFrame(defaultWidth, defaultHeight, defaultBgColor);

    connect(timeline, &FrameTimeLine::frameSelected, this, [=](int index){
        canvas->setImage(timeline->currentFrame());
    });

    canvas->setTimeline(timeline);
    timeline->_setCanvas(canvas);
    timeline->setLayerPanel(layerPanel);

    /*QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(5);
    effect->setXOffset(2);
    effect->setYOffset(2);
    effect->setColor(Qt::black);*/
    //creamos un dock que se llame "tools"
    toolDock = new QDockWidget("Tools", this);
    toolDock->setStyleSheet("");
    // toolDock->setGraphicsEffect(effect);
    //seteamos las areas permitidas para el dock (izquierda y derecha)
    toolDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    this->setStyleSheet("");
    int toolDockHeight = toolDock->height();
    toolDock->resize(300, toolDockHeight);
    toolDock->setMinimumWidth(110);
    toolDock->setMaximumWidth(110);

    //hacemos un QWidget para el contenido
    QWidget *dockContent = new QWidget();

    //creamos un QPushButton llamado penBtn, para seleccionar la pluma
    penBtn = new QToolButton(dockContent);
    //cambiamos su tamaño y posicion
    penBtn->setGeometry(10,50,100,30);

    //aplicar un icono al boton de pen
    penBtn->setIcon(QIcon(":/data/icons/brush_icon.svg"));
    penBtn->setIconSize(QSize(24,24));
    //penBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

    //creamos un QPushButton llamado eraserBtn, para seleccionar el borrador
    eraserBtn = new QToolButton(dockContent);
    //cambiamos su tamaño y posicion, igual que para penBtn
    eraserBtn->setGeometry(10,0,100,30);

    //aplicar un icono al boton de borrador
    eraserBtn->setIcon(QIcon(":/data/icons/eraser_icon.svg"));
    eraserBtn->setIconSize(QSize(24,24));
    //eraserBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);

    //creamos un boton para paintBucket
    paintBucketBtn = new QToolButton(dockContent);

    //ajustamos algunas propiedades para paintBucketBtn
    paintBucketBtn->setGeometry(10,200,100,30);
    paintBucketBtn->setIcon(QIcon(":/data/icons/paint_bucket_icon.svg"));
    paintBucketBtn->setIconSize(QSize(24,24));

    //creamos un boton para seleccionar el color del lapiz
    colorBtn = new QPushButton("", dockContent);
    //cambiamos su tamaño y posicion
    colorBtn->setGeometry(10, 100, 100,30);

    //aplicamos icono a el color picker
    //colorBtn->setIcon(QIcon(":/data/icons/color_picker_icon"));
    //colorBtn->setIconSize(QSize(24,24));
    colorBtn->setStyleSheet("background-color: #000; border: 1px solid #000000; border-radius: 2px;");

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
        QLabel *label = new QLabel(QString("Pen size (px): "), dialog);
        QLineEdit *lineEdit = new QLineEdit(dialog);
        lineEdit->setValidator(new QDoubleValidator(0.1, 100.0, 2,lineEdit));
        lineEdit->setText(QString::number(canvas->penWidth));

        QPushButton *acceptBtn = new QPushButton("Accept", dialog);
        acceptBtn->setFixedHeight(30);

        layout->addWidget(label);
        layout->addWidget(lineEdit);
        layout->addSpacing(10);
        layout->addWidget(acceptBtn, 0, Qt::AlignCenter);

       /* connect(slider, &QSlider::valueChanged, this, [=](int value){
            label->setText(QString("Pen size: 1%").arg(value));
        });
*/
        connect(acceptBtn, &QPushButton::clicked, this, [=](){
            bool ok;
            int newSize = lineEdit->text().toDouble(&ok);
            if(ok && newSize >= 1){
                canvas->penWidth = newSize;
            }
            if(newSize >100){
                QMessageBox msgBox;
                msgBox.setText("You can't set a number higher than 100");
                msgBox.setStyleSheet("");
                msgBox.exec();
                canvas->penWidth = 100;
            }
            if(canvas->penWidth <= 0){
                QMessageBox msgBox;
                msgBox.setText("You can't set a number lower than 1.00");
                msgBox.setStyleSheet("");
                msgBox.exec();
                canvas->penWidth = 1.0;
            }

            dialog->accept();
        });

        contentWidget->setLayout(layout);
        QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
        mainLayout->addWidget(contentWidget);
        dialog->setLayout(mainLayout);

        dialog->exec();
    });

    connect(paintBucketBtn, &QToolButton::clicked, this, [this](){
        canvas->setTool(Canvas::Bucket);
        updateToolButtons(Canvas:: Bucket);
    });
    

    // conectar el botón de color
    connect(colorBtn, &QPushButton::clicked, this, [=]() {
        QColor selectedColor = QColorDialog::getColor(canvas->penColor, this, "Select pen color");

        if (selectedColor.isValid()) {
            canvas->setPenColor(selectedColor);
            QString selectedColorStr = selectedColor.name(); // "#rrggbb"
            colorBtn->setStyleSheet(QString("background-color: %1;").arg(selectedColorStr));
        }
    });

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

    penBtn->setStyleSheet("");
    eraserBtn->setStyleSheet("");
    penSizeBtn->setStyleSheet("");
    paintBucketBtn->setStyleSheet("");


    shortcutPen = new QShortcut(QKeySequence(Qt::Key_B), this);
    connect(shortcutPen, &QShortcut::activated, this, [this]() {
    canvas->setTool(Canvas::Pen);
    updateToolButtons(Canvas::Pen);
    });

    shortcutPen = new QShortcut(QKeySequence(Qt::Key_E), this);
    connect(shortcutPen, &QShortcut::activated, this, [this]() {
    canvas->setTool(Canvas::Eraser);
    updateToolButtons(Canvas::Eraser);
    });

    shortcutPen = new QShortcut(QKeySequence(Qt::SHIFT | Qt::Key_G), this);
    connect(shortcutPen, &QShortcut::activated, this, [this]() {
    canvas->setTool(Canvas::Bucket);
    updateToolButtons(Canvas::Bucket);
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::createNewFile(){
    auto confirm = QMessageBox::question(this, "New File", "Are you sure you want to create a new file? Unsaved work will be lost");

    if(confirm != QMessageBox::Yes){
        return;
    }

    NewProjectDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted){
        int width = dialog.getWidth();
        int height = dialog.getHeight();
        QColor bgColor = dialog.getBackgroundColor();
        projectName = dialog.getProjectName();
        int fps = dialog.getFps();

        timeline->clear();

        timeline->setCurrentBgColor(bgColor);
        timeline->setCurrentHeight(height);
        timeline->setCurrentWidth(width);

        layerPanel->setNewLayerParams(width, height, bgColor);

        canvas->initializeNewCanvas(width, height, bgColor);
        layerPanel->refreshList();
        timeline->addNewFrame(width, height, bgColor);
        canvas->update();

    }
}

void MainWindow::about(){
    QMessageBox::information(this, "About", "Nothing to see here, just a simple paint app made with Qt. :)",
                             QMessageBox::Ok);
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

void MainWindow::openColorPickerDock(){
    addDockWidget(Qt::RightDockWidgetArea, colorPickerDock);
    colorPickerDock->show();
}

void MainWindow::openTimeLine(){
    addDockWidget(Qt::BottomDockWidgetArea, timeline);
    timeline->show();
}

void MainWindow::exportFn(){
    QString fileName = QFileDialog::getSaveFileName(this, "Export image",  projectName, "PNG (*.png)");
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
       penBtn->setStyleSheet("");
       eraserBtn->setStyleSheet("");
       paintBucketBtn->setStyleSheet("");
       /*QString activeStyle = R"(
            QPushButton {
                    background-color:  #394352;
                     border-radius: 2px;
             }

            QPushButton::icon {
            }

        )";
    */

       QString activeStyle = "background-color: #394352; border-radius: 2px";

       if(tool == Canvas::Pen){
           penBtn->setStyleSheet("");
           penBtn->setStyleSheet("");
       }else if (tool == Canvas::Eraser){
           eraserBtn->setStyleSheet("");
           eraserBtn->setStyleSheet("");
       }else if(tool == Canvas::Bucket){
            paintBucketBtn->setStyleSheet("");
            paintBucketBtn->setStyleSheet("");
       }
    }
}

void MainWindow::openPreferences(){
    preferences->exec();
}

void MainWindow::welcomeNewFile(){
    newProjectWindow->exec();
}
