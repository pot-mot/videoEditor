#ifndef VIDEOTIMELINE_H
#define VIDEOTIMELINE_H

#include <QWidget>
#include <QList>
#include <QPainter>
#include <QMouseEvent>
#include "../clip/ClipBase.h"  // 引入基类 Clip

QT_BEGIN_NAMESPACE

namespace Ui {
    class VideoTimeline;
}

QT_END_NAMESPACE

enum DragMode { None, Move, ResizeLeft, ResizeRight };

class VideoTimeline : public QWidget {
    Q_OBJECT

public:
    explicit VideoTimeline(QWidget *parent = nullptr);

    ~VideoTimeline() override;

    void addClip(Clip *clip);

    void removeClip(int index);

    QList<Clip *> getClips() { return clips; }

protected:
    void paintEvent(QPaintEvent *event) override;        // 绘制时间轴和 Clips
    void mousePressEvent(QMouseEvent *event) override;   // 鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;    // 鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *event) override; // 鼠标释放事件

private:
    Ui::VideoTimeline *ui;
    QList<Clip *> clips; // 存储所有 clip 数据

    double scale = 1;
    bool dragging = false;
    Clip* draggedClip = nullptr;
    QPoint dragStartPos;
    int originalStartTime = 0;
    int originalOffsetTime = 0;
    int originalDuration = 0;
    DragMode dragMode = None;

    int yPositionForClip(const Clip *clip) const;

signals:
    // 新增信号：通知 timeline 更新
    void timelineUpdated();
};

#endif //VIDEOTIMELINE_H
