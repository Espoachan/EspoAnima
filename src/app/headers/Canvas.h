#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include "Layer.h"

class Canvas;

class FrameTimeLine;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    void addLayer(int width, int height, const QColor &bgColor = Qt::transparent);
    void removeLayer(int index);
    void moveLayer(int fromIndex, int toIndex);
    void setLayerVisible(int index, bool visible);
    int layerCount() const;
    QImage compositeImage() const;
    Layer* getLayer(int index);

    bool drawing = false;
    QImage image;
    QPointF lastPoint;
    void setPenColor(const QColor &color);
    QColor penColor;
    QPointF lastPanPoint;
    void clearCanvas();

    void drawBackground(QPainter &painter, const QRect &area);

    QImage getImage() const;
    void setImage(const QImage &newImage);
    QPoint currentMousePos;

    enum Tool { Pen, Eraser, Bucket};

    Tool currentTool = Pen;

    void setTimeline(FrameTimeLine *timeline);

    QColor colorToUse = Qt::black;
    double penWidth = 40;

    void bucketFill(const QPoint &startPoint, const QColor &fillColor);
    void initializeNewCanvas(int width, int height, const QColor& bgColor);

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event) override;

    void setTool(Tool tool);

    qreal scaleFactor = 1.0;
    QPointF offset = QPointF(0,0);
    explicit Canvas(QWidget *parent = nullptr);

    int getCurrentLayerIndex() const { return currentLayerIndex; }
    void setCurrentLayerIndex(int index) { currentLayerIndex = index; }

    int currentLayerIndex = 0;



private:
    FrameTimeLine *timeline = nullptr;
    QVector<Layer> layers;


signals:

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private slots:
};

#endif // CANVAS_H
