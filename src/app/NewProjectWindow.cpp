#include "headers/NewProjectWindow.h"
#include "headers/NewProjectDialog.h"

#include <QLabel>
#include <QScreen>
#include <QGuiApplication>
#include <QGridLayout>

NewProjectWindow::NewProjectWindow(QWidget *parent) : QDialog(parent)
{
    QScreen *screen  = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    setMinimumSize(screenWidth, screenHeight);
    setGeometry(100, 100, screenWidth, screenHeight);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowTitleHint);
    setWindowTitle("Welcome!");
    setModal(true);
    setStyleSheet("");

    QGridLayout *layout = new QGridLayout(this);

    QLabel *welcomeLabel = new QLabel("Welcome to EspoAnima!", this);
    QPushButton *startProjectBtn = new QPushButton("Start new project", this);
    QPushButton *openProjectBtn = new QPushButton("Open a project", this);
    layout->addWidget(welcomeLabel, 0, 0, 1, 2, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(startProjectBtn, 1, 0, 1, 2, Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(openProjectBtn, 2, 0, 1, 2, Qt::AlignLeft | Qt::AlignTop);

    connect(startProjectBtn, &QPushButton::clicked, this, [this]() {
        NewProjectDialog *newProjectDialog = new NewProjectDialog(this);
        newProjectDialog->exec();
        if (newProjectDialog->result() == QDialog::Accepted) {
            QString projectName = newProjectDialog->getProjectName();
            int width = newProjectDialog->getWidth();
            int height = newProjectDialog->getHeight();
            int fps = newProjectDialog->getFps();
            QColor backgroundColor = newProjectDialog->getBackgroundColor();
            this->close();
        }
    });

}

