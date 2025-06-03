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

    VideoTimeline *timeline = getTimeline();
    double scale = timeline->getScale();
    int scrollLeft = timeline->getScrollLeft();
    int scrollTop = timeline->getScrollTop();
    int scrollWidth = timeline->getScrollWidth();
    int scrollHeight = timeline->getScrollHeight();
    int trackHeight = timeline->getTrackHeight();
    int trackGap = timeline->getTrackGap();

    int baseTickInterval = 100; // 每 100ms 一个刻度（基于原始时间）
    double scaledTickInterval = baseTickInterval * scale; // 根据 scale 缩放为像素间隔

    // painter.setPen(Qt::lightGray);
    // for (int x = 0; x < scrollWidth; x += scaledTickInterval) {
    //     int timeMs = static_cast<int>(x / scale); // 将像素位置转换为时间（ms）
    //
    //     painter.drawLine(x, 0, x, height()); // 绘制刻度线
    //     painter.drawText(QPoint(x + 2, 15), QString("%1ms").arg(timeMs)); // 显示时间标签
    // }

    int maxX = 0;

    int y = scrollTop;
    for (const auto &clip: timeline->getClips()) {
        int clipRectX = clip->getStartTime() * scale + scrollLeft;
        int clipRectWidth = clip->getDuration() * scale;

        QRect rect(clipRectX, y, clipRectWidth, trackHeight);
        if (clip == draggedClip) {
            painter.fillRect(rect, "#1E90FF");
            if (dragMode == DragMode::ResizeLeft) {
                QRect leftRect(clipRectX, y, 5, trackHeight);
                painter.fillRect(leftRect, Qt::red);
            } else if (dragMode == DragMode::ResizeRight) {
                QRect rightRect(clipRectX + clipRectWidth - 5, y, 5, trackHeight);
                painter.fillRect(rightRect, Qt::red);
            }
        } else {
            painter.fillRect(rect, Qt::darkGray); // 可以根据 ResourceType 设置不同颜色
        }
        painter.drawText(rect, Qt::AlignCenter, clip->getFilePath());

        int currentRight = clip->getStartTime() + clip->getDuration();
        if (currentRight > maxX) {
            maxX = currentRight;
        }
        y += trackHeight + trackGap;
    }

    scrollWidth = maxX * scale;
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
        int scrollLeft = timeline->getScrollLeft();
        int trackHeight = timeline->getTrackHeight();
        int trackGap = timeline->getTrackGap();

        int y = scrollTop;
        for (auto clip: timeline->getClips()) {
            int clipRectX = clip->getStartTime() * scale + scrollLeft;
            int clipRectWidth = clip->getDuration() * scale;
            QRect rect(clipRectX, y, clipRectWidth, trackHeight);
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
                int leftEdge = clipRectX;
                int rightEdge = clipRectX + clipRectWidth;
                if (std::abs(event->position().x() - leftEdge) < 20) {
                    dragMode = ResizeLeft;
                } else if (std::abs(event->position().x()- rightEdge) < 20) {
                    dragMode = ResizeRight;
                } else {
                    dragMode = Move;
                }
                break;
            }
        }
        update();
    }
}

void VideoTimelineTrack::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && draggedClip) {
        double scale = getTimeline()->getScale();
        int dx = event->position().x() - dragStartPos.x();
        double diff = dx / scale;
        switch (dragMode) {
            case Move:
                emit getTimeline()->beforeClipChange();
                draggedClip->setStartTime(originalStartTime + diff);
                emit getTimeline()->clipChanged();
                break;
            case ResizeLeft:
                emit getTimeline()->beforeClipChange();
                draggedClip->setStartTime(originalStartTime + diff);
                draggedClip->setOffsetTime(originalOffsetTime - diff);
                draggedClip->setDuration(originalDuration - diff);
                emit getTimeline()->clipChanged();
                break;
            case ResizeRight:
                emit getTimeline()->beforeClipChange();
                draggedClip->setDuration(originalDuration + diff);
                emit getTimeline()->clipChanged();
                break;
            default:
                break;
        }
        update();
    }
}

void VideoTimelineTrack::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        draggedClip = nullptr;
        dragMode = None;
        update();
    }
}
