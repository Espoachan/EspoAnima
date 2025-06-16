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
    void addNewFrame();
    int getCurrentIndex() const;

signals:
    void frameSelected(int index);

private:
    QVector<QImage> frames;
    QVector<QPushButton*> frameButtons;
    int currentIndex;
    QHBoxLayout *layout;
    QWidget *frameContainer;

    void UpdateUI();
    void switchToFrame();
};

#endif // FRAMETIMELINE_H
