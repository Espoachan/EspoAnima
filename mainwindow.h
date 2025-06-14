#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QScreen>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void cuandoPresionoBoton();

private:
    Ui::MainWindow *ui;
    QWidget *centralWidget;
    QPushButton *Button;
    QLabel *Label;

};
#endif // MAINWINDOW_H
