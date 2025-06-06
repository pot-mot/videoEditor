#ifndef VIDEOTIMELINE_H
#define VIDEOTIMELINE_H

#include <QWidget>
#include <QList>
#include <QPainter>
#include <QMouseEvent>
#include <QScrollBar>

#include "../clip/ClipBase.h"  // 引入基类 Clip

class VideoTimelineTrack;
class VideoEditor;
QT_BEGIN_NAMESPACE

namespace Ui {
    class VideoTimeline;
}

QT_END_NAMESPACE

class VideoTimeline : public QWidget {
    Q_OBJECT

public:
    explicit VideoTimeline(VideoEditor* editor);

    ~VideoTimeline() override;

    void addClip(Clip *clip);

    void removeClip(int index);

    void moveClip(int index, int newIndex);

    QList<Clip *> getClips() const { return clips; }

    void setTotalDuration(int duration) {
        totalDuration = duration;
        emit totalDurationChange(duration);
    }
    int getTotalDuration() const { return totalDuration; }

    int getScrollTop() const { return scrollTop; }
    int getScrollLeft() const { return scrollLeft; }
    int getScrollHeight() const { return scrollHeight; }
    int getScrollWidth() const { return scrollWidth; }
    int getTrackHeight() const { return trackHeight; }
    int getTrackGap() const { return trackGap; }
    double getScale() const { return scale; }

    void setScale(double newScale) { scale = newScale; }

    QScrollBar *getHorizontalScrollBar() const { return horizontalScrollBar; }
    QScrollBar *getVerticalScrollBar() const { return verticalScrollBar; }

    int getCurrentTime() const;
signals:
    void totalDurationChange(int duration);

    void beforeClipChange();
    void clipChanged();

    void clipSelected(Clip *clip);

protected:
    void resizeEvent(QResizeEvent *event) override; // 尺寸变化事件

private:
    VideoEditor *editor;

    Ui::VideoTimeline *ui;
    QList<Clip *> clips; // 存储所有 clip 数据

    int totalDuration = 0;

    int scrollTop = 0; // 轨道绘制区域顶部偏移量（y）
    int scrollLeft = 0; // 轨道绘制区域左侧偏移量（x）
    int scrollHeight = 0; // 完全滚动长度
    int scrollWidth = 0; // 完全滚动宽度

    QScrollBar *horizontalScrollBar; // 横向滚动条
    QScrollBar *verticalScrollBar; // 纵向滚动条

    int trackHeight = 30; // 轨道高度
    int trackGap = 10; // 轨道间隔（y）

    double scale = 0.1; // 时间缩放（以1ms为基础的缩放倍率）

    VideoTimelineTrack * track;
};

#endif //VIDEOTIMELINE_H
