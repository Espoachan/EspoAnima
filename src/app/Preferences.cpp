#include "headers/Preferences.h"
#include <QFormLayout>
#include <QDialogButtonBox>

Preferences::Preferences(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Preferences");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
    setModal(true);
    setMinimumSize(600, 400);
    setGeometry(100, 100, 600, 400);
    setStyleSheet("background-color: #222; color: white;");

    QPushButton *themeButton = new QPushButton("", this);
    themeButton->setStyleSheet("background-color: #444; color: white; border-radius: 2px;");
    themeButton->setToolTip("Change Theme");
    connect(themeButton, &QPushButton::clicked, this, &Preferences::close);
    themeButton->setGeometry(100, 400, 80, 30);

    QFormLayout *form = new QFormLayout;
    form->addRow("Theme", themeButton);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &Preferences::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &Preferences::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(form);
    mainLayout->addWidget(buttons);
    setLayout(mainLayout);
}

