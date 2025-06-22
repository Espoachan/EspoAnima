#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QPushButton>


class Preferences : public QDialog {
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = nullptr);
    void changeTheme(const QString &newTheme);

signals:

private:

};

#endif // PREFERENCES_H
