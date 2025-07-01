#include "ColorPicker.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include "../headers/Canvas.h"

ColorPicker::ColorPicker(QWidget *parent, Canvas *canvas)
    : QWidget(parent),
      hue(0.0),
      currentColor(Qt::white),            
      canvas(canvas)
{
    setMinimumSize(180, 180);
    generateHuePixmap();
    generateTrianglePixmap();

    // Inicia los selectores en posiciones por defecto
    int w = width();
    int h = height();
    QPointF center(w / 2.0, h / 2.0);

    // Selector hue al punto inicial rojo (hue=0)
    hueSelector = QPointF(center.x() + (radiusOuter() + radiusInner()) / 2, center.y());

    // Selector triángulo en centro
    triangleSelector = center;
 
}

// --- Funciones helper para radio y centro ---

int ColorPicker::radiusOuter() const {
    return qMin(width(), height()) / 2 - 10;
}

int ColorPicker::radiusInner() const {
    return radiusOuter() - 20;
}

QPointF ColorPicker::center() const {
    return QPointF(width() / 2.0, height() / 2.0);
}

// --- Genera el pixmap del anillo de tonos (hue) ---

void ColorPicker::generateHuePixmap() {
    int w = width();
    int h = height();
    QImage hueImg(w, h, QImage::Format_ARGB32);
    hueImg.fill(Qt::transparent);

    QPointF c = center();
    int rOut = radiusOuter();
    int rIn = radiusInner();

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            qreal dx = x - c.x();
            qreal dy = y - c.y();
            qreal dist = std::sqrt(dx * dx + dy * dy);

            if (dist >= rIn && dist <= rOut) {
                qreal angle = std::atan2(dy, dx);
                if (angle < 0) angle += 2 * M_PI;
                QColor c = QColor::fromHsvF(angle / (2 * M_PI), 1, 1);
                hueImg.setPixelColor(x, y, c);
            }
        }
    }
    huePixmap = QPixmap::fromImage(hueImg);
}

// --- Genera el pixmap del triángulo de saturación/valor ---

void ColorPicker::generateTrianglePixmap() {
    int w = width();
    int h = height();

    QImage triImg(w, h, QImage::Format_ARGB32);
    triImg.fill(Qt::transparent);

    QPolygonF triangle = trianglePolygon();

    // Limites rectangulares para optimizar
    QRectF triRect = triangle.boundingRect();

    for (int y = int(triRect.top()); y <= int(triRect.bottom()); ++y) {
        for (int x = int(triRect.left()); x <= int(triRect.right()); ++x) {
            QPointF p(x, y);
            if (pointInTriangle(p)) {
                // Normaliza coords dentro del triángulo (s, v)
                qreal s = (p.x() - triRect.left()) / triRect.width();
                qreal v = 1.0 - (p.y() - triRect.top()) / triRect.height();
                QColor color = QColor::fromHsvF(hue, s, v);
                triImg.setPixelColor(x, y, color);
            }
        }
    }
    trianglePixmap = QPixmap::fromImage(triImg);
}

// --- El polígono equilátero del triángulo centrado ---

QPolygonF ColorPicker::trianglePolygon() const {
    QPointF c = center();
    int size = radiusInner() * 1.4; // ajusta para que quepa dentro del círculo

    QPointF p1(c.x(), c.y() - size / std::sqrt(3));              // punta arriba
    QPointF p2(c.x() - size / 2, c.y() + size / (2 * std::sqrt(3))); // abajo izquierda
    QPointF p3(c.x() + size / 2, c.y() + size / (2 * std::sqrt(3))); // abajo derecha

    return QPolygonF() << p1 << p2 << p3;
}

// --- Comprueba si un punto está dentro del triángulo ---

bool ColorPicker::pointInTriangle(const QPointF &p) const {
    return trianglePolygon().containsPoint(p, Qt::OddEvenFill);
}

// --- Comprueba si un punto está dentro del anillo hue ---

bool ColorPicker::inHueCircle(const QPointF &p) const {
    QPointF c = center();
    qreal dist = std::hypot(p.x() - c.x(), p.y() - c.y());
    return dist >= radiusInner() && dist <= radiusOuter();
}

// --- Calcula ángulo desde el centro (en radianes, 0 a 2pi) ---

qreal ColorPicker::angleFromCenter(const QPointF &p) const {
    QPointF c = center();
    qreal angle = std::atan2(p.y() - c.y(), p.x() - c.x());
    if (angle < 0) angle += 2 * M_PI;
    return angle;
}

// --- Obtiene color del punto dentro del triángulo ---

QColor ColorPicker::colorAtPoint(const QPointF &p) const {
    QRectF triRect = trianglePolygon().boundingRect();

    qreal s = (p.x() - triRect.left()) / triRect.width();
    qreal v = 1.0 - (p.y() - triRect.top()) / triRect.height();

    return QColor::fromHsvF(hue, s, v);
}

// --- Eventos de mouse ---

void ColorPicker::mousePressEvent(QMouseEvent *event) {
    QPointF pos = event->pos();

    if (inHueCircle(pos)) {
        qreal newHue = angleFromCenter(pos) / (2 * M_PI);
        if (!qFuzzyCompare(hue, newHue)) {
            hue = newHue;

            QPointF c = center();
            QPointF delta = pos - c;
            qreal dist = std::hypot(delta.x(), delta.y());

            if (dist > radiusOuter()) {
                QPointF dir = delta / dist;
                hueSelector = c + dir * radiusOuter();
            } else if (dist < radiusInner()) {
                QPointF dir = delta / dist;
                hueSelector = c + dir * radiusInner();
            } else {
                hueSelector = pos;
            }               
            generateTrianglePixmap();  // Solo aquí porque hue cambió
            currentColor = colorAtPoint(triangleSelector);
            canvas->penColor = currentColor;
            emit colorSelected(currentColor);
            update();
        }
    } else if (pointInTriangle(pos)) {
        triangleSelector = pos;
        currentColor = colorAtPoint(pos);
        canvas->penColor = currentColor;
        emit colorSelected(currentColor);
        update();
    }
}

void ColorPicker::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton)) return;

    QPointF pos = event->pos();

    if (inHueCircle(pos)) {
        qreal newHue = angleFromCenter(pos) / (2 * M_PI);
        if (!qFuzzyCompare(hue, newHue)) {
            hue = newHue;

            QPointF c = center();
            QPointF delta = pos - c;
            qreal dist = std::hypot(delta.x(), delta.y());

            if (dist > radiusOuter()) {
                QPointF dir = delta / dist;
                hueSelector = c + dir * radiusOuter();
            } else if (dist < radiusInner()) {
                QPointF dir = delta / dist;
                hueSelector = c + dir * radiusInner();
            } else {
                hueSelector = pos;
            }

            // Sólo regenerar el triángulo si cambió el hue
            generateTrianglePixmap();
            currentColor = colorAtPoint(triangleSelector);
            canvas->penColor = currentColor;
            emit colorSelected(currentColor);
            update();
        }
    } else if (pointInTriangle(pos)) {
        if (pointInTriangle(pos)) {
            triangleSelector = pos;
            currentColor = colorAtPoint(triangleSelector);
            canvas->penColor = currentColor;
            emit colorSelected(currentColor);
            update();
        }
    }
}


// --- PaintEvent simple, solo dibuja pixmaps y selectores ---

void ColorPicker::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawPixmap(0, 0, huePixmap);
    painter.drawPixmap(0, 0, trianglePixmap);

    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(hueSelector, 7, 7);
    painter.drawEllipse(triangleSelector, 7, 7);
}

void ColorPicker::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    // Regenera pixmaps solo si cambió tamaño
    generateHuePixmap();
    generateTrianglePixmap();

    // Recalcula posición de selectores para que estén dentro del área
    int w = width();
    int h = height();
    QPointF c = center();

    // Posiciona hueSelector en el ángulo actual hue y radio medio
    qreal angle = hue * 2 * M_PI;
    qreal r = (radiusOuter() + radiusInner()) / 2;
    hueSelector = QPointF(c.x() + r * std::cos(angle), c.y() + r * std::sin(angle));

    // Posiciona triangleSelector en centro del triángulo si no está inicializado
    if (!pointInTriangle(triangleSelector)) {
        triangleSelector = c;
    }

    update();
}
