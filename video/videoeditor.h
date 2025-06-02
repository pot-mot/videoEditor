#ifndef VIDEOEDITOR_H
#define VIDEOEDITOR_H

#include <QLabel>
#include <QMainWindow>
#include <QWidget>
#include <QVideoWidget>
#include <QSlider>
#include "../file/fileviewer.h"
#include "../video/timeline/videotimeline.h"

namespace Ui {
class VideoEditor;
}

class VideoEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoEditor(QWidget *parent = nullptr);
    ~VideoEditor();

private:
    Ui::VideoEditor *ui;

    void initUI();

    double fps;
    int width;
    int height;

    FileViewer *fileResourceTree;
    QLabel *videoPreview;
    QSlider *mainTimeline;
    VideoTimeline *sliceTimeline;
};

#endif // VIDEOEDITOR_H
