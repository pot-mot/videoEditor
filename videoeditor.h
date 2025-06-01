#ifndef VIDEOEDITOR_H
#define VIDEOEDITOR_H

#include <QMainWindow>
#include <QWidget>
#include <QTreeWidget>
#include <QVideoWidget>
#include <QSlider>
#include <QToolButton>

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

    QTreeWidget *fileResourceList;
    QVideoWidget *videoPreview;
    QSlider *mainTimeline;
    QSlider *sliceTimeline;
    QToolButton *functionMenu;

    void openVideo();
    void exportVideo();
    void setVideo(const QImage& video);
    void resetVideo();
};

#endif // VIDEOEDITOR_H
