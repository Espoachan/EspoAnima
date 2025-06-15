#include "Canvas.h"
#include <QMouseEvent>
#include <QPainter>
#include <QKeyEvent>
#include <QRgb>
#include <QStack>

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
{
    image = QImage(1280, 720, QImage::Format_ARGB32);
    image.fill(Qt::white);

    setFocusPolicy(Qt::StrongFocus);
}
void Canvas::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        drawing = true;
        QPointF canvasPos = (event->pos() - offset) / scaleFactor;
        lastPoint = canvasPos.toPoint();
    }

    if (event->button() == Qt::RightButton){
        lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }

    if (currentTool == Bucket && event->button() == Qt::LeftButton) {
        QPoint imgPoint = ((event->pos() - offset) / scaleFactor).toPoint();
        bucketFill(imgPoint, penColor);
        return; // ya no hacer más nada
    }

}

void Canvas::mouseMoveEvent(QMouseEvent *event){


    if(currentTool == Bucket){
        return;
    }
    if(currentTool == Pen){
        colorToUse = penColor;
    }else if (currentTool == Eraser){
        colorToUse = Qt::white;
    }

    if((event->buttons() & Qt::LeftButton) && drawing){
         QPointF canvasPos = (event->pos() - offset) / scaleFactor;

        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(colorToUse, penWidth, Qt::SolidLine, Qt::RoundCap));
        painter.drawLine(lastPoint, canvasPos);
        lastPoint = canvasPos.toPoint();
        update();
    }

    if(event->buttons() & Qt::RightButton){
        QPointF delta = event->pos() - lastPanPoint;
        offset += delta;
        lastPanPoint = event->pos();
        update();
    }

    currentMousePos = event->pos();
    update();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && drawing){
        drawing = false;
    }

    if(event->button() == Qt::RightButton){
        setCursor(Qt::ArrowCursor);
    }
}

void Canvas::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);

    painter.translate(offset);
    painter.scale(scaleFactor, scaleFactor);

    painter.drawImage(0,0,image);

    QPointF adjustedPos = (currentMousePos - offset) / scaleFactor;
    int x  = adjustedPos.x();
    int y = adjustedPos.y();

    if (x  >= 0 && x < image.width() && 7 >= 0 && y < image.height()){
        QColor pixelColor = image.pixelColor(x, y);
        QColor invertedColor(255 - pixelColor.red(),
                                                255 - pixelColor.green(),
                                                255 - pixelColor.blue());
        QPen previewPen(invertedColor);
        previewPen.setStyle(Qt::SolidLine);
        previewPen.setWidth(1);
        painter.setPen(previewPen);
        painter.setBrush(Qt::NoBrush);
    }
    qreal radius = penWidth / 2.0;
    painter.drawEllipse(adjustedPos, radius, radius);
}

void Canvas::wheelEvent(QWheelEvent *event){
    constexpr qreal zoomInFactor = 1.1;
    constexpr qreal zoomOutFactor = 1.0 / zoomInFactor;

    if(event->angleDelta().y() >0){
        scaleFactor *= zoomInFactor;
    }
    else{
        scaleFactor *= zoomOutFactor;
    }

    update();
}

void Canvas::setPenColor(const QColor &color) {
    penColor = color;
}

void Canvas::keyPressEvent(QKeyEvent *event){
    if(event->modifiers() & Qt::ControlModifier){
        if(event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal){
            scaleFactor *= 1.1;
            update();
        }else if  (event->key() == Qt::Key_Minus){
                scaleFactor /= 1.1;
                update();
        }
    }

    QWidget::keyPressEvent(event);
}

void Canvas::clearCanvas(){
    image.fill(Qt::white);
    update();
}

void Canvas::setTool(Tool tool){
    currentTool = tool;

}

void Canvas::bucketFill(const QPoint &startPoint, const QColor &fillColor)
{
    QRgb targetColor = image.pixel(startPoint);
    QRgb replacementColor = fillColor.rgb();

    if (targetColor == replacementColor){
        return;
    }

    QStack<QPoint> stack;
    stack.push(startPoint);

    while (!stack.isEmpty()){
        QPoint pt = stack.pop();

        if(image.rect().contains(pt) && image.pixel(pt) == targetColor){
            image.setPixel(pt, replacementColor);

            stack.push(QPoint(pt.x() + 1, pt.y()));
            stack.push(QPoint(pt.x() - 1, pt.y()));
            stack.push(QPoint(pt.x(), pt.y() + 1));
            stack.push(QPoint(pt.x(), pt.y() - 1));
        }
    }

    update();
}
