#ifndef NEW_PROJECT_WINDOW_H
#define NEW_PROJECT_WINDOW_H  

#include <QPushButton>
#include <QDialog>

class NewProjectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectWindow(QWidget *parent = nullptr);

private:
    QPushButton *createNewProjectButton;


private:
    // Private members and methods
};

#endif // NEW_PROJECT_WINDOW_H