#include "headers/Preferences.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include "headers/globals.h"
#include <QApplication>


Preferences::Preferences(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Preferences");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
    setModal(true);
    setMinimumSize(600, 400);
    setGeometry(100, 100, 600, 400);
    setStyleSheet("");

    static bool dark = true;
    QPushButton *themeButton = new QPushButton("", this);
    if(dark){
        themeButton->setText("Dark theme");
        qApp->setStyleSheet(theme);
    }
    //themeButton->setCheckable(false);
    themeButton->setStyleSheet("");
    themeButton->setToolTip("Change Theme");
    connect(themeButton, &QPushButton::clicked, this, [=]() {
        dark = !dark;

        if (dark) {
            theme = R"(
                QWidget {
                    background-color: #222;
                    color: white;
                }

                QPushButton {
                    background-color: #444;
                    color: white;
                    border: 1px solid #888;
                    padding: 5px;
                }

                QPushButton:hover {
                    background-color: #666;
                }

                QToolButton {
                    background-color: #444;
                    color: black;
                    border: 1px solid #888;
                    padding: 5px;
                }

                QToolButton:hover {
                    background-color: #666;
                }
            )";
            qApp->setStyleSheet(theme);
            themeButton->setText("Dark theme");
        }else {
            theme = R"(
                QWidget {
                    background-color: #fff;
                    color: black;
                }

                QPushButton {
                    background-color: #b8b8b8;
                    color: black;
                    border: 1px solid #222;
                    padding: 5px;
                }

                QPushButton:hover {
                    background-color: #666;
                }

                QToolButton {
                    background-color: #b8b8b8;
                    color: black;
                    border: 1px solid #222;
                    padding: 5px;
                }

                QToolButton:hover {
                    background-color: #666;
                }
            )";
            qApp->setStyleSheet(theme);
            themeButton->setText("Light theme");
    }
});

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

void Preferences::changeTheme(const QString &newTheme) {
    
}

