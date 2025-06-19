#include "headers/Preferences.h"

Preferences::Preferences(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Preferences");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
    setModal(true);
    setMinimumSize(400, 300);

    // Add your preference UI elements here
    // For example, you can add buttons, checkboxes, etc.
    QPushButton *closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &Preferences::close);
    closeButton->setGeometry(10, 10, 80, 30);
}

