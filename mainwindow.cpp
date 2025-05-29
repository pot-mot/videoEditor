#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QLabel>
#include <QPixmap>
#include <QSlider>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QImage>
#include <QColor>
#include <QScrollArea>
#include <QScroller>
#include <QDebug>
#include "imageprocessor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initUI() {
    // 创建 QScrollArea 并设置为中央部件
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setWidgetResizable(true); // 图片随滚动区域缩放

    scrollArea->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    setCentralWidget(scrollArea);

    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QToolBar* toolBar = addToolBar(tr("工具栏"));

    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    scrollArea->setWidget(imageLabel);

    QToolBar* scaleToolBar = new QToolBar(this);
    scaleToolBar->setWindowTitle(tr("缩放工具栏"));
    addToolBar(Qt::BottomToolBarArea, scaleToolBar);

    // 添加缩放滑块到底部工具栏
    scaleToolBar->addWidget(new QLabel(tr("缩放比例:"), this));
    scaleSlider = new QSlider(Qt::Horizontal, this);
    scaleSlider->setRange(10, 200);
    scaleSlider->setValue(100);
    scaleToolBar->addWidget(scaleSlider);
    connect(scaleSlider, &QSlider::valueChanged, this, [this]() {
        qreal scaleFactor = scaleSlider->value() / 100.0;
        QImage scaledImage = this->originImage.scaled(
            this->originImage.size() * scaleFactor,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );
        this->setImage(scaledImage);
    });

    QMenu* fileMenu = menuBar->addMenu(tr("文件"));
    QMenu* processMenu = menuBar->addMenu(tr("图像处理"));
    QMenu* helpMenu = menuBar->addMenu(tr("帮助"));

    QAction* openAction = fileMenu->addAction(tr("打开图片"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openImage);
    toolBar->addAction(openAction);

    QAction* resetAction = fileMenu->addAction(tr("初始化"));
    connect(resetAction, &QAction::triggered, this, &MainWindow::resetImage);
    toolBar->addAction(resetAction);

    QAction* grayscaleAction = processMenu->addAction(tr("灰度化"));
    connect(grayscaleAction, &QAction::triggered, this, [this]() {
        this->setImage(ImageProcessor::grayscaleImage(this->getCurrentImage()));
    });
    toolBar->addAction(grayscaleAction);

    QAction* binarizeAction = processMenu->addAction(tr("二值化"));
    connect(binarizeAction, &QAction::triggered, this, [this]() {
        this->setImage(ImageProcessor::binarizeImage(this->getCurrentImage(), this->thresholdSlider->value()));
    });
    toolBar->addAction(binarizeAction);
    thresholdSlider = new QSlider(Qt::Horizontal, this);
    thresholdSlider->setRange(0, 255);
    thresholdSlider->setValue(128);
    connect(thresholdSlider, &QSlider::valueChanged, this, [this]() {
        this->setImage(ImageProcessor::binarizeImage(this->getCurrentImage(), this->thresholdSlider->value()));
    });

    QAction* meanFilterAction = processMenu->addAction(tr("3x3均值滤波"));
    connect(meanFilterAction, &QAction::triggered, this, [this]() {
        this->setImage(ImageProcessor::meanFilterImage(this->getCurrentImage()));
    });
    toolBar->addAction(meanFilterAction);

    QAction* gammaCorrectionAction = processMenu->addAction(tr("伽马变换"));
    connect(gammaCorrectionAction, &QAction::triggered, this, [this]() {
        this->setImage(ImageProcessor::gammaCorrection(this->getCurrentImage()));
    });
    toolBar->addAction(gammaCorrectionAction);

    QAction* edgeDetectionAction = processMenu->addAction(tr("边缘检测"));
    connect(edgeDetectionAction, &QAction::triggered, this, [this]() {
        this->setImage(ImageProcessor::edgeDetection(this->getCurrentImage()));
    });
    toolBar->addAction(edgeDetectionAction);

    QAction* aboutAction = helpMenu->addAction(tr("关于"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
}

void MainWindow::openImage() {
    QString imagePath = QFileDialog::getOpenFileName(this, tr("打开图片"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!imagePath.isEmpty()) {
        this->originImage = QImage(imagePath);

        QSize windowSize = this->size();
        QSize targetSize = windowSize * 0.7;

        qreal widthFactor = (qreal)targetSize.width() / (qreal)originImage.width();
        qreal heightFactor = (qreal)targetSize.height() / (qreal)originImage.height();
        qreal scaleFactor = qMin(widthFactor, heightFactor);

        scaleSlider->setValue(qRound(scaleFactor * 100));
    }
}

QImage MainWindow::getCurrentImage() {
    return this->imageLabel->pixmap().toImage();
}

void MainWindow::resetImage() {
    if (this->originImage.isNull()) {
        QMessageBox::warning(this, tr("警告"), tr("未打开图片"));
        return;
    }

    qreal scaleFactor = scaleSlider->value() / 100.0;
    QImage scaledImage = this->originImage.scaled(
        this->originImage.size() * scaleFactor,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    this->setImage(scaledImage);
}

void MainWindow::setImage(const QImage& image) {
    imageLabel->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::showAboutDialog() {
    QMessageBox::about(this, tr("关于"), tr("数字图像处理软件V1.0\n作者：李钺\n学号：249400220"));
}
