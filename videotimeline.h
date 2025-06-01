//
// Created by 37702 on 2025/6/1.
//

#ifndef VIDEOTIMELINE_H
#define VIDEOTIMELINE_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class VideoTimeline; }
QT_END_NAMESPACE

class VideoTimeline : public QWidget {
Q_OBJECT

public:
    explicit VideoTimeline(QWidget *parent = nullptr);
    ~VideoTimeline() override;

private:
    Ui::VideoTimeline *ui;
};


#endif //VIDEOTIMELINE_H
