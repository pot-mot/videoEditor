#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include <QImage>
#include <QPixmap>
#include "commandhistory.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


private:
    Ui::MainWindow *ui;

    CommandHistory commandHistory;

    QLabel* imageLabel;
    QSlider* thresholdSlider;
    QSlider* scaleSlider;
    QImage originImage;
    QImage currentImage;

    void initUI();
    void showAboutDialog();

    void openImage();
    void setImage(const QImage& image);
    void resetImage();
};

#endif // MAINWINDOW_H
