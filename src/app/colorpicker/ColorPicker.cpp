#include "ColorPicker.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

ColorPicker::ColorPicker(QWidget *parent)
    : QWidget(parent), selectorPos(50, 50)
{
    setMinimumSize(150, 150);
}

void ColorPicker::setHue(qreal hue)
{
    hue = hue;
    update();
}

QColor ColorPicker::selectedColor() const
{
    return currentColor;
}

void ColorPicker::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPolygonF triangle = trianglePolygon();

    QImage img(width(), height(), QImage::Format_ARGB32);
    img.fill(Qt::white);

    for(int y = 0; y < height(); ++y) {
        for(int x = 0; x < width(); ++x) {
            QPointF p(x, y);
            if(pointInTriangle(p)) {
                qreal s = qreal(x) / width();
                qreal v = 1.0 - qreal(y) / height();
                QColor color = QColor::fromHsvF(hue, s, v);
                img.setPixel(x, y, color.rgb());
            }
        }
    }
    
    painter.drawImage(0, 0, img);

    painter.setPen(Qt::black);
    painter.drawEllipse(selectorPos, 5, 5);
}

void ColorPicker::mousePressEvent(QMouseEvent *event)
{
    if (pointInTriangle(event->pos())) {
        selectorPos = event->pos();
        currentColor = colorAtPoint(selectorPos);
        emit colorSelected(currentColor);
        update();
    } 
}

void ColorPicker::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && pointInTriangle(event->pos())) {
        selectorPos = event->pos();
        currentColor = colorAtPoint(selectorPos);
        emit colorSelected(currentColor);
        update();
    }
}

QPolygonF ColorPicker::trianglePolygon() const
{
    QPointF p1(width() / 2, 10);
    QPointF p2(10, height() - 10);
    QPointF p3(width() - 10, height() - 10);
    return QPolygonF() << p1 << p2 << p3;
}

bool ColorPicker::pointInTriangle(const QPointF &p) const
{
    return trianglePolygon().containsPoint(p, Qt::OddEvenFill);
}

QColor ColorPicker::colorAtPoint(const QPointF &p) const
{
    qreal s = qBound(0.0, p.x() / width(), 1.0);
    qreal v = qBound(0.0, 1.0 - p.y() / height(), 1.0);

    return QColor::fromHsvF(hue, s, v);
}