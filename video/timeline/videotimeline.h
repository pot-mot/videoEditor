#ifndef VIDEOTIMELINE_H
#define VIDEOTIMELINE_H

#include <QWidget>
#include <QList>
#include "../clip/clipbase.h"  // 引入基类 Clip

QT_BEGIN_NAMESPACE
namespace Ui { class VideoTimeline; }
QT_END_NAMESPACE

class VideoTimeline : public QWidget {
    Q_OBJECT

public:
    explicit VideoTimeline(QWidget *parent = nullptr);
    ~VideoTimeline() override;

    // 新增方法：添加 clip
    void addClip(Clip *clip);

    // 新增方法：移除 clip
    void removeClip(int index);

private:
    Ui::VideoTimeline *ui;
    QList<Clip*> clips;         // 存储所有 clip 数据

    // 新增方法：更新 timeline
    void updateTimeline();

signals:
    // 新增信号：通知 timeline 更新
    void timelineUpdated();
};

#endif //VIDEOTIMELINE_H