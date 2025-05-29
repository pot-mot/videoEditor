#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include <QImage>
#include <QPixmap>

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
    QLabel* imageLabel;
    QSlider* thresholdSlider;
    QSlider* scaleSlider;
    QImage originImage;

    void initUI();
    void showAboutDialog();

    void openImage();
    QImage getCurrentImage();
    void setImage(const QImage& image);
    void resetImage();
};

#endif // MAINWINDOW_H
