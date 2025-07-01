#include "headers/Preferences.h"
#include "headers/globals.h"

#include <QFormLayout>
#include <QDialogButtonBox>
#include <QApplication>
#include <QSettings>


Preferences::Preferences(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Preferences");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
    setModal(true);
    setMinimumSize(600, 400);
    setGeometry(100, 100, 600, 400);
    setStyleSheet("");
    QSettings* settings = new QSettings("Espoachan", "EspoAnima");
    QString savedTheme = settings->value("theme", "light").toString();
    static bool dark;
    if (savedTheme == "dark")
    {
        dark = true;
    }else {
        dark = false;
    }
    QPushButton *themeButton = new QPushButton("", this);
    if(dark){
        themeButton->setText("Dark theme");
        qApp->setStyleSheet(theme);
    }else {
        themeButton->setText("Light theme");
        qApp->setStyleSheet(R"(
            QWidget {
                background-color: #c2c2c2;
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
        )");
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
            settings->setValue("theme", "dark"); // Guarda la cadena "dark" con la clave "theme"

        }else {
            theme = R"(
                QWidget {
                    background-color: #c2c2c2;
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
            settings->setValue("theme", "light"); // Guarda la cadena "light" con la clave "theme"
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

