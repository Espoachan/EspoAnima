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

    QImage getImage() const;
    void setImage(const QImage &newImage);
    QPoint currentMousePos;

    enum Tool { Pen, Eraser, Bucket};

    Tool currentTool = Pen;

    QColor colorToUse;
    double penWidth;

    void bucketFill(const QPoint &startPoint, const QColor &fillColor);

    void setTool(Tool tool);

    qreal scaleFactor = 1.0;
    QPointF offset = QPointF(0,0);
    explicit Canvas(QWidget *parent = nullptr);

private:
    FrameTimeLine *timeline = nullptr;

signals:

protected:
    void setTimeline(FrameTimeLine *timeline);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private slots:
};

#endif // CANVAS_H
