#ifndef VIDEOTIMELINE_H
#define VIDEOTIMELINE_H

#include <QWidget>
#include <QList>
#include "../clip/ClipBase.h"  // 引入基类 Clip

QT_BEGIN_NAMESPACE

namespace Ui {
    class VideoTimeline;
}

QT_END_NAMESPACE

class VideoTimeline : public QWidget {
    Q_OBJECT

public:
    explicit VideoTimeline(QWidget *parent = nullptr);

    ~VideoTimeline() override;

    void addClip(Clip *clip);

    void removeClip(int index);

    QList<Clip *> getClips() { return clips; }

private:
    Ui::VideoTimeline *ui;
    QList<Clip *> clips; // 存储所有 clip 数据

signals:
    // 新增信号：通知 timeline 更新
    void timelineUpdated();
};

#endif //VIDEOTIMELINE_H
