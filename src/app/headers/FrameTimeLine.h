#ifndef FRAMETIMELINE_H
#define FRAMETIMELINE_H

#pragma once
#include <QDockWidget>
#include <QList>
#include <QVector>
#include <QPushButton>
#include <QToolButton>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QImage>
#include <QWidget>

class FrameTimeLine : public QDockWidget
{
    Q_OBJECT

public:
    explicit FrameTimeLine(QWidget *parent = nullptr);
    const QImage& currentFrame() const;
    void setFrame(int index, const QImage& image);
    void addNewFrame(int width, int height, QColor color);
        void addNewFrame();


    int getCurrentIndex() const;
    QPushButton *addFrameBtn;
    void clear();

    void setCurrentWidth(int w);
    void setCurrentHeight(int h);
    void setCurrentBgColor(QColor c);

signals:
    void frameSelected(int index);

private:
    QVector<QImage> frames;
    QVector<QPushButton*> frameButtons;
    int currentIndex;
    QHBoxLayout *layout;
    QWidget *frameContainer;

    int currentWidth = 1280;  // valores por defecto
    int currentHeight = 720;
    QColor currentBgColor = Qt::white;

    void UpdateUI();
    void switchToFrame(int index);
};

#endif // FRAMETIMELINE_H
