#ifndef VIDEOEDITOR_H
#define VIDEOEDITOR_H

#include <QMainWindow>
#include <QWidget>
#include <QVideoWidget>
#include <QSlider>
#include <QToolButton>
#include "fileviewer.h"

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

    FileViewer *fileResourceTree;
    QVideoWidget *videoPreview;
    QSlider *mainTimeline;
    QSlider *sliceTimeline;
    QToolButton *functionMenu;
};

#endif // VIDEOEDITOR_H
