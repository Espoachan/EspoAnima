#include "headers/LayerPanel.h"
#include "headers/Canvas.h"

LayerPanel::LayerPanel(Canvas* canvas, QWidget* parent)
    : QDockWidget("Capas", parent), canvas(canvas)
{
    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);

    layerList = new QListWidget(this);
    layout->addWidget(layerList);

    QHBoxLayout* btnLayout = new QHBoxLayout();

    addBtn = new QPushButton("+", this);
    removeBtn = new QPushButton("-", this);
    upBtn = new QPushButton("⬆", this);
    downBtn = new QPushButton("⬇", this);
    toggleVisibilityBtn = new QPushButton("👁", this);

    btnLayout->addWidget(addBtn);
    btnLayout->addWidget(removeBtn);
    btnLayout->addWidget(upBtn);
    btnLayout->addWidget(downBtn);
    btnLayout->addWidget(toggleVisibilityBtn);

    layout->addLayout(btnLayout);
    container->setLayout(layout);
    setWidget(container);

    connect(addBtn, &QPushButton::clicked, this, &LayerPanel::addLayer);
    connect(addBtn, &QPushButton::clicked, this, &LayerPanel::removeLayer);
    connect(upBtn, &QPushButton::clicked, this, &LayerPanel::moveLayerUp);
    connect(downBtn, &QPushButton::clicked, this, &LayerPanel::moveLayerDown);
    connect(toggleVisibilityBtn, &QPushButton::clicked, this, &LayerPanel::toggleVisibility);
    connect(layerList, &QListWidget::currentRowChanged, [=](int row){
        canvas->setCurrentLayerIndex(row);
    });

    refreshList();
}

void LayerPanel::refreshList() {
    layerList->clear();
    for (int i = 0; i < canvas->layerCount(); ++i) {
        Layer* layer = canvas->getLayer(i);
        if (layer) {
            QString name = layer->getName().isEmpty() ? QString("Capa %1").arg(i + 1) : layer->getName();
            if (!layer->isVisible()) name += " (oculta)";
            layerList->addItem(name);
        }
    }

    layerList->setCurrentRow(canvas->getCurrentLayerIndex());
}

void LayerPanel::addLayer() {
    canvas->addLayer(1280, 720, Qt::transparent);  // Puedes ajustar tamaño/color
    refreshList();
}

void LayerPanel::removeLayer() {
    int index = layerList->currentRow();
    if (index >= 0) {
        canvas->removeLayer(index);
        refreshList();
    }
}

void LayerPanel::moveLayerUp() {
    int index = layerList->currentRow();
    if (index > 0) {
        canvas->moveLayer(index, index - 1);
        refreshList();
        layerList->setCurrentRow(index - 1);
    }
}

void LayerPanel::moveLayerDown() {
    int index = layerList->currentRow();
    if (index >= 0 && index < canvas->layerCount() - 1) {
        canvas->moveLayer(index, index + 1);
        refreshList();
        layerList->setCurrentRow(index + 1);
    }
}

void LayerPanel::toggleVisibility() {
    int index = layerList->currentRow();
    if (index >= 0) {
        Layer* layer = canvas->getLayer(index);
        if (layer) {
            layer->setVisible(!layer->isVisible());
            refreshList();
        }
    }
}
