#include "headers/NewProjectDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QColorDialog>

NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent), backgroundColor(Qt::white)
{
    // Set the dialog properties
    setMinimumSize(400, 300);
    setGeometry(100, 100, 400, 300);
    setStyleSheet("background-color: #222; color: white;");
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
    setWindowTitle("New Project");

    nameEdit = new QLineEdit(this);
    widthSpin = new QSpinBox(this);
    widthSpin->setRange(1, 10000);
    widthSpin->setValue(1280);

    heightSpin = new QSpinBox(this);
    heightSpin->setRange(1,10000);
    heightSpin->setValue(720);

    fpsSpin = new QSpinBox(this);
    fpsSpin->setRange(1, 120);
    fpsSpin->setValue(24);

    colorButton = new QPushButton("Select Color", this);
    connect(colorButton, &QPushButton::clicked, this, &NewProjectDialog::chooseColor);

    QFormLayout *form = new QFormLayout;
    form->addRow("Project name:", nameEdit);
    form->addRow("Width (px):", widthSpin);
    form->addRow("Height (px):", heightSpin);
    form->addRow("FPS:", fpsSpin);
    form->addRow("Background color:", colorButton);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &NewProjectDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &NewProjectDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(form);
    mainLayout->addWidget(buttons);
    setLayout(mainLayout);
}

void NewProjectDialog::chooseColor(){
    QColor color = QColorDialog::getColor(backgroundColor, this);
    if(color.isValid()){
        backgroundColor = color;
    }
}

QString NewProjectDialog::getProjectName() const {
    return nameEdit->text();
}

int NewProjectDialog::getWidth() const {
    return widthSpin->value();
}

int NewProjectDialog::getHeight() const {
    return heightSpin->value();
}

int NewProjectDialog::getFps() const {
    return fpsSpin->value();
}

QColor NewProjectDialog::getBackgroundColor() const {
    return backgroundColor;
}
