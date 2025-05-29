#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QImage>
#include <QPixmap>
#include "commandhistory.h"

namespace Ui {
class ImageEditor;
}

class ImageEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageEditor(QWidget *parent = nullptr);
    ~ImageEditor();

private:
    Ui::ImageEditor *ui;

    CommandHistory commandHistory;

    QLabel* imageLabel;
    QSlider* thresholdSlider;
    QSlider* scaleSlider;
    QImage originImage;
    QImage currentImage;

    void initUI();

    void openImage();
    void setImage(const QImage& image);
    void resetImage();
};

#endif // IMAGEEDITOR_H
