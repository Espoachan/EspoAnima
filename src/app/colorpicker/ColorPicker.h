#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QWidget>
#include <QColor>
#include <QPolygonF>
#include <QPixmap>

class Canvas;

class ColorPicker : public QWidget
{
    Q_OBJECT

public:
    //explicit ColorPicker(QWidget *parent = nullptr);
    explicit ColorPicker(QWidget *parent = nullptr, Canvas* canvas = nullptr);
    void setCanvas(Canvas* canvas);

    QColor selectedColor() const { return currentColor; }
    void setHue(qreal hue);
    QColor currentColor;  

signals:
    void colorSelected(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    qreal hue;              // valor actual del tono [0..1]
    Canvas* canvas = nullptr;

    QPointF hueSelector;      // posición del selector en el círculo
    QPointF triangleSelector; // posición del selector en el triángulo

    QPixmap huePixmap;       // pixmap cacheado del círculo de tonos
    QPixmap trianglePixmap;  // pixmap cacheado del triángulo de s/v

    // Helpers para geometría
    int radiusOuter() const;
    int radiusInner() const;
    QPointF center() const;

    void generateHuePixmap();
    void generateTrianglePixmap();

    QPolygonF trianglePolygon() const;

    bool pointInTriangle(const QPointF &p) const;
    bool inHueCircle(const QPointF &p) const;

    qreal angleFromCenter(const QPointF &p) const;

    QColor colorAtPoint(const QPointF &p) const;

private slots:
    // Slots no necesarios aquí, manejo directo con eventos de mouse

};

#endif // COLORPICKER_H
