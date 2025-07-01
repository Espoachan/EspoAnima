#include "headers/menubarhelper.h"
#include <headers/mainwindow.h>

#include <QMessageBox>

QMenuBar* menuBarHelper::createMenuBar(QMainWindow *parent){

    //Creamos una barra de menu superior
    QMenuBar *menuBar = new QMenuBar(parent);

    //creamos un menu llamado "Files"
    QMenu *menuFiles = new QMenu("Files", parent);
    //creamos un menu llamado "Edit"
    QMenu *menuEdit = new QMenu("Edit", parent);
    //creamos un menu llamado "Tools"
    QMenu *menuTools = new QMenu("Tools", parent);
    //creamos otro llamado "Help"
    QMenu *menuHelp = new QMenu("Help", parent);
    
    //agregamos un QAction a el menu "Files", esta accion se llama "New File"
    QAction *newFile = menuFiles->addAction("New File");
    QAction *_toolsDock = menuTools->addAction("Tools Dock");
    QAction *about = menuHelp->addAction("About");
    QAction *_colorPickerDock = menuTools->addAction("Color Picker");
    QAction *_timelineDock = menuTools->addAction("Timeline Dock");

    //creamos mas acciones para el menu de "Files" pero no se utilizan por el momento
    QAction *saveAs = menuFiles->addAction("Save as");
    QAction *save = menuFiles->addAction("Save");
    QAction *openFile = menuFiles->addAction("Open File");
    QAction *_export = menuFiles->addAction("Export");
    QAction *preferences = menuEdit->addAction("Preferences");

    //le decimos al compilador que las acciones no se usan en ningun momento aproposito
    Q_UNUSED(openFile);
    Q_UNUSED(save);
    Q_UNUSED(saveAs);


    //si se selecciona la accion new file, se llama a la funcion "createNewFile"
    QObject::connect(newFile, &QAction::triggered, [=](){static_cast<MainWindow*>(parent)->createNewFile();});
    QObject::connect(about, &QAction::triggered, static_cast<MainWindow*>(parent), &MainWindow::about);
    QObject::connect(_toolsDock, &QAction::triggered, [=](){static_cast<MainWindow*>(parent)->openToolsDock();});
    QObject::connect(_export, &QAction::triggered, [=](){static_cast<MainWindow*>(parent)->exportFn();});
    QObject::connect(preferences, &QAction::triggered, [=](){static_cast<MainWindow*>(parent)->openPreferences();});
    QObject::connect(_colorPickerDock, &QAction::triggered, [=](){static_cast<MainWindow*>(parent)->openColorPickerDock();});
    QObject::connect(_timelineDock, &QAction::triggered, [=](){static_cast<MainWindow*>(parent)->openTimeLine();});
    // mostramos el menuFiles
    menuBar->addMenu(menuFiles);
    // agregamos el menu "Edit" a la barra de menu
    menuBar->addMenu(menuEdit);
    // agregamos el menu "Tools" a la barra de menu
    menuBar->addMenu(menuTools);
    // agregamos el menu "Help" a la barra de menu
    menuBar->addMenu(menuHelp);

    return menuBar;
}
