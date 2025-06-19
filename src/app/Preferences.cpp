#include "headers/Preferences.h"

Preferences::Preferences(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Preferences");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
    setModal(true);
    setMinimumSize(600, 400);
    setGeometry(100, 100, 600, 400);
    setStyleSheet("background-color: #222; color: white;");

    // Create a close button
    // This button will close the preferences dialog when clickeds
    // and has a fixed size of 80x30 pixels.
    QPushButton *closeButton = new QPushButton("Close", this);
    closeButton->setStyleSheet("background-color: #444; color: white; border-radius: 2px;");
    closeButton->setToolTip("Close Preferences");
    connect(closeButton, &QPushButton::clicked, this, &Preferences::close);
    closeButton->setGeometry(100, 100, 80, 30);
}

