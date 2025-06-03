//
// Created by 37702 on 2025/6/3.
//

#ifndef VIDEOTIMELINETRACK_H
#define VIDEOTIMELINETRACK_H

#include <QWidget>

#include <QWidget>
#include <QList>
#include <QPainter>
#include <QMouseEvent>
#include <QScrollBar>

#include "videotimeline.h"
#include "../clip/ClipBase.h"  // 引入基类 Clip

QT_BEGIN_NAMESPACE
namespace Ui { class VideoTimelineTrack; }
QT_END_NAMESPACE

enum DragMode { None, Move, ResizeLeft, ResizeRight };

class VideoTimelineTrack : public QWidget {
Q_OBJECT

public:
    explicit VideoTimelineTrack(VideoTimeline *parent = nullptr);
    ~VideoTimelineTrack() override;

    VideoTimeline* getTimeline() const { return static_cast<VideoTimeline *>(parent()); }

protected:
    void paintEvent(QPaintEvent *event) override;        // 绘制时间轴和 Clips
    void mousePressEvent(QMouseEvent *event) override;   // 鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;    // 鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event) override; // 鼠标释放事件

private:
    Ui::VideoTimelineTrack *ui;
    int timeHeight = 20;
    int paddingBottom = 40;

    bool dragging = false;
    Clip* draggedClip = nullptr;
    QPoint dragStartPos;
    int originalStartTime = 0;
    int originalOffsetTime = 0;
    int originalDuration = 0;
    DragMode dragMode = None;
};


#endif //VIDEOTIMELINETRACK_H
