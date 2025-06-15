#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QScreen>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>
#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include "Canvas.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    void createNewFile();
    void about();
    void openToolsDock();
    void exportFn();
    void updateToolButtons(Canvas::Tool tool);


    QPushButton *penBtn;
    QPushButton *colorBtn;
    QPushButton *eraserBtn;
    QPushButton *penSizeBtn;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

private:
    Ui::MainWindow *ui;
    QWidget *centralWidget;
    QDockWidget *toolDock;
    Canvas *canvas; // agrégalo como miembro privado
};
#endif // MAINWINDOW_H
