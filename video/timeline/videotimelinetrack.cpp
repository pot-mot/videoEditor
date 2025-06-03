//
// Created by 37702 on 2025/6/3.
//

// You may need to build the project (run Qt uic code generator) to get "ui_VideoTimelineTrack.h" resolved

#include "videotimelinetrack.h"
#include "ui_VideoTimelineTrack.h"


VideoTimelineTrack::VideoTimelineTrack(VideoTimeline *parent) : QWidget(parent), ui(new Ui::VideoTimelineTrack) {
    ui->setupUi(this);
}

VideoTimelineTrack::~VideoTimelineTrack() {
    delete ui;
}

void VideoTimelineTrack::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    QPainter painter(this);

    // 绘制时间轴背景
    // painter.setPen(Qt::lightGray);
    // for (int x = 0; x < width(); x += 50) {
    //     painter.drawLine(x, 0, x, height());
    // }

    int maxX = 0;
    VideoTimeline *timeline = getTimeline();
    double scale = timeline->getScale();
    int scrollLeft = timeline->getScrollLeft();
    int scrollTop = timeline->getScrollTop();
    int scrollWidth = timeline->getScrollWidth();
    int scrollHeight = timeline->getScrollHeight();
    int trackHeight = timeline->getTrackHeight();
    int trackGap = timeline->getTrackGap();

    int y = scrollTop;
    for (const auto &clip: timeline->getClips()) {
        int xStart = clip->getStartTime() * scale + scrollLeft;
        int clipWidth = clip->getDuration() * scale;

        QRect rect(xStart, y, clipWidth, trackHeight);
        painter.fillRect(rect, Qt::blue); // 可以根据 ResourceType 设置不同颜色
        painter.drawText(rect, Qt::AlignCenter, clip->getFilePath());

        if (clipWidth + xStart > maxX) {
            maxX = clipWidth + xStart;
        }
        y += trackHeight + trackGap;
    }

    scrollWidth = maxX;
    scrollHeight = y;

    timeline->getHorizontalScrollBar()->setRange(0, scrollWidth - width());
    timeline->getVerticalScrollBar()->setRange(0, scrollHeight - height());

    qDebug() << "scrollWidth: " << scrollWidth << ", scrollHeight: " << scrollHeight;
}

void VideoTimelineTrack::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        VideoTimeline *timeline = getTimeline();
        double scale = timeline->getScale();
        int scrollTop = timeline->getScrollTop();
        int trackHeight = timeline->getTrackHeight();
        int trackGap = timeline->getTrackGap();

        int y = scrollTop;
        for (auto clip: timeline->getClips()) {
            int xStart = clip->getStartTime() * scale;
            int widthClip = clip->getDuration() * scale;
            QRect rect(xStart, y, widthClip, trackHeight);
            y += trackHeight + trackGap;

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
                if (std::abs(event->position().x() - leftEdge) < 10) {
                    dragMode = ResizeLeft;
                } else if (std::abs(event->position().x() - rightEdge) < 10) {
                    dragMode = ResizeRight;
                } else {
                    dragMode = Move;
                }
                break;
            }
        }
    }
}

void VideoTimelineTrack::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && draggedClip) {
        int scale = getTimeline()->getScale();
        int dx = event->position().x() - dragStartPos.x();
        switch (dragMode) {
            case Move:
                draggedClip->setStartTime(originalStartTime + dx / scale);
                break;
            case ResizeLeft:
                draggedClip->setOffsetTime(originalOffsetTime - dx / scale);
                draggedClip->setDuration(originalDuration + dx / scale);
                break;
            case ResizeRight:
                draggedClip->setDuration(originalDuration + dx / scale);
                break;
            default:
                break;
        }
        update(); // 触发重绘
    }
}

void VideoTimelineTrack::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        draggedClip = nullptr;
        dragMode = None;
    }
}
