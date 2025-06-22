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


    void refreshList();

    
};

#endif // LAYERPANEL_H