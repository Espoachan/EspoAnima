#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include <QPushButton>
#include <QMessageBox>

class Preferences : public QObject {
    Q_OBJECT

public:
    explicit Preferences(QObject *parent = nullptr);

signals:

private:

};

#endif // PREFERENCES_H
