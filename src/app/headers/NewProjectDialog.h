#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QColor>
#include <QPushButton>

class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = nullptr);

    QString getProjectName() const;
    int getWidth() const;
    int getHeight() const;
    int getFps() const;
    QColor getBackgroundColor() const;

private slots:
    void chooseColor();

private:
    QLineEdit *nameEdit;
    QSpinBox *widthSpin;
    QSpinBox *heightSpin;
    QSpinBox *fpsSpin;
    QPushButton *colorButton;
    QColor backgroundColor;
};

#endif // NEWPROJECTDIALOG_H
