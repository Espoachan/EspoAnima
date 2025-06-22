#include "headers/Layer.h"

Layer::Layer(int width, int height, const QColor &bgColor)
    : image(width, height, QImage::Format_ARGB32), visible(true), name("Layer") 
{
    image.fill(bgColor);
}

QImage& Layer::getImage() {
    return image;
}

const QImage& Layer::getImage() const {
    return image;
}

void Layer::setVisible(bool v) {
    visible = v;
}

void Layer::setName(const QString &n) {
    name = n;
}

QString Layer::getName() const {
    return name;
}

bool Layer::isVisible() const {
    return visible;
}
