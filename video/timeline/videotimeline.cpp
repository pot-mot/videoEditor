//
// Created by 37702 on 2025/6/1.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoTimeline.h" resolved

#include "videotimeline.h"
#include "ui_VideoTimeline.h"
#include "videotimelinetrack.h"

VideoTimeline::VideoTimeline(QWidget *parent) : QWidget(parent), ui(new Ui::VideoTimeline) {
    ui->setupUi(this);

    horizontalScrollBar = new QScrollBar(Qt::Horizontal, this);
    horizontalScrollBar->setFixedHeight(10);
    verticalScrollBar = new QScrollBar(Qt::Vertical, this);
    verticalScrollBar->setFixedWidth(10);

    horizontalScrollBar->setRange(0, scrollWidth); // 可根据 scrollWidth 动态调整
    verticalScrollBar->setRange(0, scrollHeight);   // 可根据 scrollHeight 动态调整

    horizontalScrollBar->setValue(scrollLeft);
    verticalScrollBar->setValue(scrollTop);

    connect(horizontalScrollBar, &QScrollBar::valueChanged, this, [this](int value) {
        scrollLeft = -value;
        update();
    });

    connect(verticalScrollBar, &QScrollBar::valueChanged, this, [this](int value) {
        scrollTop = -value;
        update();
    });

    track = new VideoTimelineTrack(this);
    connect(track, &VideoTimelineTrack::clipSelected, this, [this](Clip *clip) {
        emit this->clipSelected(clip);
    });
}

VideoTimeline::~VideoTimeline() {
    delete ui;
}

// 实现 addClip 方法
void VideoTimeline::addClip(Clip *clip) {
    emit this->beforeClipChange();
    clips.append(clip);
    update();
    emit this->clipChanged();
}

// 实现 removeClip 方法
void VideoTimeline::removeClip(int index) {
    if (index >= 0 && index < clips.size()) {
        emit this->beforeClipChange();
        clips.removeAt(index);
        update();
        emit this->clipChanged();
    }
}

void VideoTimeline::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    // 横向滚动条：宽度与父容器一致，固定在底部
    horizontalScrollBar->setGeometry(0, height() - horizontalScrollBar->height(), width(), horizontalScrollBar->height());

    // 纵向滚动条：高度与父容器一致，固定在右侧
    verticalScrollBar->setGeometry(width() - verticalScrollBar->width(), 0, verticalScrollBar->width(), height());

    track->setGeometry(0, 0, width() - verticalScrollBar->width(), height() - horizontalScrollBar->height());
}
