//
// Created by 37702 on 2025/6/1.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoTimeline.h" resolved

#include "videotimeline.h"
#include "ui_VideoTimeline.h"

VideoTimeline::VideoTimeline(QWidget *parent) :
    QWidget(parent), ui(new Ui::VideoTimeline) {
    ui->setupUi(this);
}

VideoTimeline::~VideoTimeline() {
    delete ui;
}

// 实现 addClip 方法
void VideoTimeline::addClip(Clip *clip) {
    clips.append(clip);
}

// 实现 removeClip 方法
void VideoTimeline::removeClip(int index) {
    if (index >= 0 && index < clips.size()) {
        clips.removeAt(index);
    }
}
