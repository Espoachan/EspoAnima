#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QColor>
#include <QPolygonF>

class ColorPicker : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPicker(QWidget *parent = nullptr);
    void setHue(qreal hue);
    QColor selectedColor() const;

signals:
    void colorSelected(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    qreal hue = 0.0;
    QPointF selectorPos;
    QColor currentColor;

    QPolygonF trianglePolygon() const;
    bool pointInTriangle(const QPointF &p) const;
    QColor colorAtPoint(const QPointF &p) const;
};

#endif // COLORPICKER_H