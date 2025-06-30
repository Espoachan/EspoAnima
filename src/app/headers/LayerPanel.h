#ifndef LAYERPANEL_H
#define LAYERPANEL_H
#pragma once

#include <QDockWidget>
#include <QListWidget>
#include <QPushButton> 
#include <QVBoxLayout>
#include <QHBoxLayout>

class Canvas;

class LayerPanel : public QDockWidget
{
    Q_OBJECT
public:
    LayerPanel(Canvas* canvas, QWidget *parent = nullptr);
    void setNewLayerParams(int width, int height, const QColor &bgColor);
    void refreshList();
    void deselectLayer();

private slots:
    void addLayer();
    void removeLayer();
    void moveLayerUp();
    void moveLayerDown();
    void toggleVisibility();

private:
    Canvas* canvas;
    QListWidget* layerList;
    QPushButton* addBtn;
    QPushButton* removeBtn;
    QPushButton* upBtn;
    QPushButton* downBtn;
    QPushButton* toggleVisibilityBtn;

    int newLayerWidth = 1280;
    int newLayerHeight = 720;
    QColor newLayerBgColor = Qt::transparent;
};

#endif // LAYERPANEL_H