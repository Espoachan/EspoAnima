#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QScreen>
#include <QShortcut>
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
#include <QToolButton>
#include <FrameTimeLine.h>
#include <QString>
#include <Preferences.h>

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
//funciones
    void createNewFile();
    void about();
    void openToolsDock();
    void exportFn();
    void updateToolButtons(Canvas::Tool tool);
    void openPreferences();
//

//estas son de otros.h
    FrameTimeLine *timeline;
    Preferences *preferences;

//
    QString resolutionAnswer1;
    QString resolutionAnswer2;
    QToolButton *penBtn;
    QPushButton *colorBtn;
    QToolButton *eraserBtn;
    QPushButton *penSizeBtn;
    QToolButton *paintBucketBtn;
    QShortcut *shortcutPen;
    QString projectName = "Untitled Project";



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
