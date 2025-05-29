#ifndef VIDEOEDITOR_H
#define VIDEOEDITOR_H

#include <QWidget>

namespace Ui {
class VideoEditor;
}

class VideoEditor : public QWidget
{
    Q_OBJECT

public:
    explicit VideoEditor(QWidget *parent = nullptr);
    ~VideoEditor();

private:
    Ui::VideoEditor *ui;
};

#endif // VIDEOEDITOR_H
