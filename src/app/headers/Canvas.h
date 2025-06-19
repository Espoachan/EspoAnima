#ifndef CANVAS_H
#define CANVAS_H

#include "FrameTimeLine.h"
#include <QWidget>

class Canvas : public QWidget
{
    Q_OBJECT
public:
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
    double penWidth = 5.0;

    void bucketFill(const QPoint &startPoint, const QColor &fillColor);
    void initializeNewCanvas(int width, int height, const QColor& bgColor);


    void setTool(Tool tool);

    qreal scaleFactor = 1.0;
    QPointF offset = QPointF(0,0);
    explicit Canvas(QWidget *parent = nullptr);

private:
    FrameTimeLine *timeline = nullptr;

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
