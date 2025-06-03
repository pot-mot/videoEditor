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

    int totalDuration = 0;
    int y = timeHeight + scrollTop;

    for (const auto &clip: timeline->getClips()) {
        int currentRight = clip->getStartTime() + clip->getDuration();
        if (currentRight > totalDuration) {
            totalDuration = currentRight;
        }
        y += trackHeight + trackGap;
    }

    scrollWidth = totalDuration * scale;
    scrollHeight = y + paddingBottom;
    timeline->setTotalDuration(totalDuration);

    timeline->getHorizontalScrollBar()->setRange(0, scrollWidth - width());
    timeline->getVerticalScrollBar()->setRange(0, scrollHeight - height());

    // 显示时间刻度
    int tickInterval = 1000; // 初始基准间隔为 1000ms

    double visibleTickCount;
    do {
        visibleTickCount = static_cast<double>(width()) / (tickInterval * scale);

        if (visibleTickCount > 20) {
            tickInterval *= 2; // 刻度过密，增大间隔
        } else if (visibleTickCount < 4) {
            tickInterval /= 2; // 刻度过疏，减小间隔
        }
    } while (visibleTickCount > 20 || visibleTickCount < 4);

    painter.setPen(Qt::lightGray);
    for (int tick = 0; tick < totalDuration; tick += tickInterval) {
        double x = tick * scale + scrollLeft;
        painter.drawLine(x, 0, x, height()); // 绘制刻度线
        painter.drawText(QPoint(x, 15), QString("%1ms").arg(tick)); // 显示时间标签
    }

    // 绘制 Clip
    y = timeHeight + scrollTop;

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
        } else if (clip == selectedClip) {
            painter.fillRect(rect, "#3274B8");
        } else {
            painter.fillRect(rect, Qt::darkGray);
        }
        painter.drawText(rect, Qt::AlignCenter, clip->getFilePath());
        y += trackHeight + trackGap;
    }
}

void VideoTimelineTrack::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        VideoTimeline *timeline = getTimeline();
        double scale = timeline->getScale();
        int scrollTop = timeline->getScrollTop();
        int scrollLeft = timeline->getScrollLeft();
        int trackHeight = timeline->getTrackHeight();
        int trackGap = timeline->getTrackGap();

        int y = timeHeight + scrollTop;
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
        if (draggedClip != nullptr) {
            selectedClip = draggedClip;
            emit clipSelected(selectedClip);
        } else {
            selectedClip = nullptr;
            emit clipSelected(selectedClip);
        }
        dragging = false;
        draggedClip = nullptr;
        dragMode = None;
        update();
    }
}
