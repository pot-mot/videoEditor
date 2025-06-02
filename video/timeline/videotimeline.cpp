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
    update();
}

// 实现 removeClip 方法
void VideoTimeline::removeClip(int index) {
    if (index >= 0 && index < clips.size()) {
        clips.removeAt(index);
        update();
    }
}

void VideoTimeline::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);

    // 绘制时间轴背景（例如网格线）
    painter.setPen(Qt::lightGray);
    for (int x = 0; x < width(); x += 50) {
        painter.drawLine(x, 0, x, height());
    }

    int y = 20;
    for (const auto &clip : clips) {
        int xStart = clip->getStartTime() * scale; // scale 为像素/单位时间
        int widthClip = clip->getDuration() * scale;

        QRect rect(xStart, y, widthClip, 30);
        painter.fillRect(rect, Qt::blue); // 可以根据 ResourceType 设置不同颜色
        painter.drawText(rect, Qt::AlignCenter, clip->getFilePath());

        y += 40; // 下一个轨道
    }
}

void VideoTimeline::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        for (auto clip : clips) {
            int xStart = clip->getStartTime() * scale;
            int widthClip = clip->getDuration() * scale;
            QRect rect(xStart, yPositionForClip(clip), widthClip, 30);

            // 判断点击是否在 Clip 内
            if (rect.contains(event->pos())) {
                dragging = true;
                draggedClip = clip;
                dragStartPos = event->pos();
                originalStartTime = clip->getStartTime();
                originalOffsetTime = clip->getOffsetTime();
                originalDuration = clip->getDuration();

                // 判断是头部还是尾部拖动
                int leftEdge = xStart;
                int rightEdge = xStart + widthClip;
                if (std::abs(event->x() - leftEdge) < 10) {
                    dragMode = ResizeLeft;
                } else if (std::abs(event->x() - rightEdge) < 10) {
                    dragMode = ResizeRight;
                } else {
                    dragMode = Move;
                }
                break;
            }
        }
    }
}

void VideoTimeline::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && draggedClip) {
        int dx = event->x() - dragStartPos.x();
        switch (dragMode) {
            case Move:
                draggedClip->setStartTime(originalStartTime + dx / scale);
            break;
            case ResizeLeft:
                draggedClip->setOffsetTime(originalOffsetTime - dx);
            draggedClip->setDuration(originalDuration + dx);
            break;
            case ResizeRight:
                draggedClip->setDuration(originalDuration + dx);
            break;
            default:
                break;
        }
        update(); // 触发重绘
    }
}

void VideoTimeline::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        draggedClip = nullptr;
        dragMode = None;
        emit timelineUpdated();
    }
}

int VideoTimeline::yPositionForClip(const Clip *clip) const {
    int index = clips.indexOf(const_cast<Clip*>(clip)); // 找到当前 Clip 的索引
    return 20 + index * 40; // 每个轨道间隔 40px
}
