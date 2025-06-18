#include "headers/FrameTimeLine.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include "headers/NewProjectDialog.h"

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
}

void FrameTimeLine::addNewFrame(int width, int height, QColor color) {

    QImage newFrame(currentWidth,currentHeight, QImage::Format_ARGB32);
    newFrame.fill(currentBgColor);
    frames.append(newFrame);
    
    QPushButton *btn = new QPushButton(QString::number(frames.size()), frameContainer);
    btn->setFixedSize(60,60);

   // int index = frameButtons.size();

    connect(btn, &QPushButton::clicked,  this, [=]() {
        switchToFrame(frameButtons.indexOf(btn));
    });

    frameButtons.append(btn);
    layout->addWidget(btn);

    currentIndex = frames.size() - 1;
    UpdateUI();
    emit frameSelected(currentIndex);
}

void FrameTimeLine::switchToFrame(int index) {
    if(index < 0 || index >= frames.size()) return;
    currentIndex = index;
    UpdateUI();
    emit frameSelected(index);
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
        frames[index] = image;
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
