#include "headers/FrameTimeLine.h"
#include "headers/NewProjectDialog.h"
#include "headers/Canvas.h"
#include "headers/LayerPanel.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPainter>

FrameTimeLine::FrameTimeLine(QWidget *parent) : QDockWidget("Timeline", parent), currentIndex(0){
    frameContainer = new QWidget;
    layout = new QHBoxLayout(frameContainer);
    layout->setContentsMargins(5,5,5,5);
    layout->setSpacing(10);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(frameContainer);

    setWidget(scrollArea);


    addFrameBtn = new QPushButton("+", frameContainer);
    addFrameBtn->setFixedSize(60,60);
    layout->addWidget(addFrameBtn);

    //NewProjectDialog dialog;
    int width = 1280;
    int height = 720;
    QColor bgColor = Qt::white; // Default background color
    QString projectName = "untitled project";
    int fps = 24;
    connect(addFrameBtn, &QPushButton::clicked, this, [=]() {
        addNewFrame(width, height, bgColor);
    });
}

void FrameTimeLine::setCurrentWidth(int w) {
    currentWidth = w;
}

void FrameTimeLine::setCurrentHeight(int h) {
    currentHeight = h;
}

void FrameTimeLine::setCurrentBgColor(QColor c) {
    currentBgColor = c;
}

void FrameTimeLine::addNewFrame() {
    addNewFrame(currentWidth, currentHeight, currentBgColor);
    canvas->setCurrentLayerIndex(-1);
    layerPanel->deselectLayer();

}

void FrameTimeLine::addNewFrame(int width, int height, QColor color) {
    if(!canvas) return;

    int layerCount = canvas->layerCount();

    QVector<QImage> layerImages;

    for (int i = 0; i < layerCount; ++i) {
        Layer* layer = canvas->getLayer(i);
        QImage img(width, height, QImage::Format_ARGB32);
        img.fill(Qt::transparent);
        layerImages.append(img);
    }

    frameLayerContents.append(layerImages);

    /*QImage newFrame(currentWidth,currentHeight, QImage::Format_ARGB32);
    newFrame.fill(currentBgColor);
    frames.append(newFrame);*/

    QPushButton *btn = new QPushButton(QString::number(frameLayerContents.size()), frameContainer);
    btn->setFixedSize(60,60);

    // int index = frameButtons.size();

    connect(btn, &QPushButton::clicked, this, [=]() {
        switchToFrame(frameButtons.indexOf(btn));
        if (canvas && layerPanel) {
            canvas->setCurrentLayerIndex(-1);
            layerPanel->deselectLayer();
        }
    });

    frameButtons.append(btn);
    layout->addWidget(btn);

    //currentIndex = frames.size() - 1;
    currentIndex = frameLayerContents.size() - 1;
    switchToFrame(currentIndex);
    /*UpdateUI();
    emit frameSelected(currentIndex);*/
}

void FrameTimeLine::switchToFrame(int index) {
    if(index < 0 || index >= frameLayerContents.size()) return;
    
    QVector<QImage> currentImages;
    for (int i = 0; i < canvas->layerCount(); ++i) {
        Layer *layer = canvas->getLayer(i);
        if (layer) currentImages.append(layer->getImage().copy()); 
    }
    frameLayerContents[currentIndex] = currentImages;

    QVector<QImage> newImages = frameLayerContents[index];
    for (int i = 0; i < newImages.size(); ++i) {
        Layer *layer = canvas->getLayer(i);
        if (layer) {
            layer->getImage() = newImages[i].copy();
        }
    }

    currentIndex = index;
    if (layerPanel) layerPanel->refreshList();
    emit frameSelected(index);
    emit frameSwitched();
    UpdateUI();

}

void FrameTimeLine::UpdateUI() {
    for (int i = 0; i < frameButtons.size(); i++){
        if (i == currentIndex){
            frameButtons[i]->setStyleSheet("border: 2px solid blue;");
        }else{
            frameButtons[i]->setStyleSheet("");
        }
    }
}

const QImage& FrameTimeLine::currentFrame() const {
    return frames[currentIndex];
}

void FrameTimeLine::setFrame(int index, const QImage& image) {
    if (index >= 0 && index < frames.size()){
        frames[index] = image.copy();
    }
}
int FrameTimeLine::getCurrentIndex() const {
    return currentIndex;
}

void FrameTimeLine::clear(){
    for (QPushButton* btn : frameButtons){
        layout->removeWidget(btn);
        btn->deleteLater();
    }

    frameButtons.clear();
    frames.clear();
    currentIndex = 0;

    UpdateUI();
}

void FrameTimeLine::_setCanvas(Canvas* c) { canvas = c; }
void FrameTimeLine::setLayerPanel(LayerPanel* lp) { layerPanel = lp; }