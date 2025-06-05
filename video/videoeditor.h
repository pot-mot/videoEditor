#ifndef VIDEOEDITOR_H
#define VIDEOEDITOR_H

#include <QLabel>
#include <QMainWindow>
#include <QWidget>
#include <QVideoWidget>
#include <QSlider>
#include <QTimer>
#include "../file/fileviewer.h"
#include "../video/timeline/videotimeline.h"
#include "clip/ClipBase.h"
#include "clip/form/ClipForm.h"

namespace Ui {
class VideoEditor;
}

class VideoEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoEditor(QWidget *parent = nullptr);
    ~VideoEditor();

    int getCurrentTime() const {
        if (mainTimeline)
            return mainTimeline->value();
        return 0;
    }

    void setFPS(double newFPS) {
        fps = newFPS;
        interval = 1000.0 / fps;
        playTimer->setInterval(interval);
    }

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::VideoEditor *ui;

    void initUI();

    double fps;
    double interval;

    int width;
    int height;

    QTimer* playTimer = new QTimer(this);      // 播放定时器
    bool isPlaying = false;
    double playSpeed = 1.0; // 播放速度

    FileViewer *fileResourceTree;
    QLabel *videoPreview;
    QSlider *mainTimeline;
    VideoTimeline *sliceTimeline;
    ClipForm* clipForm;

    Clip * selectedClip;

    void preview();
};

#endif // VIDEOEDITOR_H
