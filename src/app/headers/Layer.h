#ifndef LAYER_H
#define LAYER_H
#pragma once

#include <QImage>
#include <QString>
#include <QColor>

class Layer {
public:
    Layer(int width, int height, const QColor &bgColor = Qt::white);

    QImage& getImage();
    const QImage& getImage() const;
    void setVisible(bool visible);
    bool isVisible() const;

    void setName(const QString &name);
    QString getName() const;

private:
    QImage image;
    bool visible;
    QString name;

};

#endif // LAYER_H