#ifndef MENUBARHELPER_H
#define MENUBARHELPER_H

#include <QMenuBar>
#include <QAction>
#include <QMainWindow>

class menuBarHelper
{
public:
    menuBarHelper();
    static QMenuBar* createMenuBar(QMainWindow *parent);
};

#endif // MENUBARHELPER_H
