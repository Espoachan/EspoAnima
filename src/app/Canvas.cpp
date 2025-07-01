#include "headers/Canvas.h"
#include "headers/FrameTimeLine.h"
#include "headers/NewProjectDialog.h"
#include "colorpicker/ColorPicker.h"

#include <QMouseEvent>
#include <QPainter>
#include <QKeyEvent>
#include <QRgb>
#include <QStack>

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}
{
 /*
    image = QImage(1280, 720, QImage::Format_ARGB32);
    image.fill(Qt::white);
*/
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

bool mouseInside = false;

void Canvas::enterEvent(QEvent *event) {
    Q_UNUSED(event);
    mouseInside = true;
    update();
}

void Canvas::leaveEvent(QEvent *event) {
    Q_UNUSED(event);
    mouseInside = false;
    update();
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
    if(currentTool != Bucket) {
        if (currentLayerIndex < 0 || currentLayerIndex >= layers.size()) return;
        // Aquí dibujas líneas si estás con lápiz o borrador
        if(currentTool == Pen){
            colorToUse = penColor;
        } else if(currentTool == Eraser){
            colorToUse = Qt::transparent;
        }

        if((event->buttons() & Qt::LeftButton) && drawing){
            
            Layer &layer = layers[currentLayerIndex];
            QPainter painter(&layer.getImage());
            QPointF canvasPos = (event->pos() - offset) / scaleFactor;

            if(currentTool == Eraser){
                // Si es la herramienta de borrado, usamos un color transparente
                // para borrar el trazo
                painter.setCompositionMode(QPainter::CompositionMode_Clear);
                painter.setPen(QPen(Qt::transparent, penWidth, Qt::SolidLine, Qt::RoundCap));
            } else {
                // Si es lápiz, usamos el color seleccionado
                painter.setRenderHint(QPainter::Antialiasing);
                painter.setPen(QPen(colorToUse, penWidth, Qt::SolidLine, Qt::RoundCap));
            }
            painter.drawLine(lastPoint, canvasPos);
            lastPoint = canvasPos.toPoint();
            update();
        }
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

        if(timeline){
            timeline->setFrame(timeline->getCurrentIndex(), layers[currentLayerIndex].getImage());
        }
    }

    if(event->button() == Qt::RightButton){
        setCursor(Qt::ArrowCursor);
    }

}

void Canvas::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);

    drawBackground(painter, rect());


    painter.translate(offset);
    painter.scale(scaleFactor, scaleFactor);

    for (const Layer &layer : layers) {
        if (layer.isVisible()) {
            painter.drawImage(0, 0, layer.getImage());
        }
    }

    QPointF adjustedPos = (currentMousePos - offset) / scaleFactor;
    int x  = adjustedPos.x();
    int y = adjustedPos.y();

    if (x  >= 0 && x < image.width() && y >= 0 && y < image.height()){
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

QImage Canvas::getImage() const {
    if (layers.isEmpty()) return QImage();
    return layers[currentLayerIndex].getImage();
}

void Canvas::setImage(const QImage &newImage) {
    if (currentLayerIndex >= 0 && currentLayerIndex < layers.size()) {
        layers[currentLayerIndex].getImage() = newImage.copy();  // Aquí se aplica el cambio real
        update();
    }
}

void Canvas::setTimeline(FrameTimeLine *timelineDock){
    timeline = timelineDock;
}

void Canvas::initializeNewCanvas(int width, int height, const QColor &bgColor)
{
    layers.clear();  // borra capas anteriores si las hay
    addLayer(width, height, bgColor);  // agrega una nueva capa visible
    currentLayerIndex = 0;
    update();
}

void Canvas::drawBackground(QPainter &painter, const QRect &area)
{

    int titleSize = 10;
    QColor lightgray(200, 200, 200);
    QColor darkgray(150, 150, 150);

    for (int y = 0; y < area.height(); y += titleSize) {
        for (int x = 0; x < area.width(); x += titleSize) {
            bool isDark = ((x / titleSize) + (y / titleSize)) % 2 == 0;
            painter.fillRect(x, y, titleSize, titleSize,
                             isDark ? darkgray : lightgray);
        }
    }
    
}

void Canvas::addLayer(int width, int height, const QColor &bgColor) {
    layers.append(Layer(width, height, bgColor));
    currentLayerIndex = layers.size() - 1;
    update();
}

void Canvas::removeLayer(int index) {
    if(index >= 0 && index < layers.size()) {
        layers.remove(index);
        if(currentLayerIndex >= layers.size()) {
            currentLayerIndex = layers.size() - 1;
        }
        update();
    }
}

void Canvas::moveLayer(int fromIndex, int toIndex) {
    if(fromIndex >= 0 && fromIndex < layers.size() && toIndex >= 0 && toIndex < layers.size() && fromIndex != toIndex) {
        Layer layer = layers.takeAt(fromIndex);
        layers.insert(toIndex, layer);
        update();
    }
}

void Canvas::setLayerVisible(int index, bool visible) {
    if(index >= 0 && index < layers.size()) {
        layers[index].setVisible(visible);
        update();
    }
}

int Canvas::layerCount() const {
    return layers.size();
}

Layer* Canvas::getLayer(int index) {
    if(index >= 0 && index < layers.size()) {
        return &layers[index];
    }
    return nullptr;
}

QImage Canvas::compositeImage() const {
    QImage result(image.width(), image.height(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    for(const Layer &layer : layers) {
        if(layer.isVisible()) {
            painter.drawImage(0, 0, layer.getImage());
        }
    }
    return result;
}