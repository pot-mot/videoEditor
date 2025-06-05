#include "imageeditor.h"
#include "ui_imageeditor.h"

#include <QFileDialog>
#include <QLabel>
#include <QPixmap>
#include <QSlider>
#include <QMessageBox>
#include <QToolBar>
#include <QAction>
#include <QImage>
#include <QScrollArea>
#include <QScroller>

#include "imageocr.h"
#include "imageprocessor.h"

ImageEditor::ImageEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ImageEditor)
{
    ui->setupUi(this);
    initUI();
}

ImageEditor::~ImageEditor()
{
    delete ui;
}

void ImageEditor::initUI() {
    QToolBar* toolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, toolBar);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);

    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    QToolBar* scaleToolBar = new QToolBar(this);
    addToolBar(Qt::BottomToolBarArea, scaleToolBar);

    scaleToolBar->addWidget(new QLabel(tr("缩放比例:"), this));
    scaleSlider = new QSlider(Qt::Horizontal, this);
    scaleSlider->setRange(10, 200);
    scaleSlider->setValue(100);
    scaleToolBar->addWidget(scaleSlider);
    connect(scaleSlider, &QSlider::valueChanged, this, [this]() {
        this->setImage(this->currentImage);
    });

    QAction* openAction = new QAction(tr("打开图片"), this);
    connect(openAction, &QAction::triggered, this, &ImageEditor::openImage);
    toolBar->addAction(openAction);

    QAction* exportAction = new QAction(tr("导出图片"), this);
    connect(exportAction, &QAction::triggered, this, &ImageEditor::exportImage);
    toolBar->addAction(exportAction);

    QAction* resetAction = new QAction(tr("初始化"), this);
    connect(resetAction, &QAction::triggered, this, &ImageEditor::resetImage);
    toolBar->addAction(resetAction);

    QAction* grayscaleAction = new QAction(tr("灰度化"), this);
    connect(grayscaleAction, &QAction::triggered, this, [this]() {
        this->setImage(ImageProcessor::grayscaleImage(this->currentImage));
    });
    toolBar->addAction(grayscaleAction);

    QAction* binarizeAction = new QAction(tr("二值化"), this);
    connect(binarizeAction, &QAction::triggered, this, [this]() {
        this->setImage(ImageProcessor::binarizeImage(this->currentImage, this->thresholdSlider->value()));
    });
    toolBar->addAction(binarizeAction);
    thresholdSlider = new QSlider(Qt::Horizontal);
    thresholdSlider->setFixedWidth(128);
    thresholdSlider->setRange(0, 255);
    thresholdSlider->setValue(127);
    toolBar->addWidget(thresholdSlider);

    QAction* meanFilterAction = new QAction(tr("3x3均值滤波"), this);
    connect(meanFilterAction, &QAction::triggered, this, [this]() {
        this->setImage(ImageProcessor::meanFilterImage(this->currentImage));
    });
    toolBar->addAction(meanFilterAction);

    QAction* gammaCorrectionAction = new QAction(tr("伽马变换"), this);
    connect(gammaCorrectionAction, &QAction::triggered, this, [this]() {
        this->setImage(ImageProcessor::gammaCorrection(this->currentImage));
    });
    toolBar->addAction(gammaCorrectionAction);

    QAction* edgeDetectionAction = new QAction(tr("边缘检测"), this);
    connect(edgeDetectionAction, &QAction::triggered, this, [this]() {
        this->setImage(ImageProcessor::edgeDetection(this->currentImage));
    });
    toolBar->addAction(edgeDetectionAction);

    QAction* ocrAction = new QAction(tr("文字识别"), this);
    connect(ocrAction, &QAction::triggered, this, [this]() {
         QMessageBox::information(this, tr("识别结果"), ImageOCR::getText(this->currentImage));
    });
    toolBar->addAction(ocrAction);
}

void ImageEditor::openImage() {
    QString imagePath = QFileDialog::getOpenFileName(this, tr("打开图片"), "", tr("Image Files (*.png *.jpg *.jpeg)"));
    if (!imagePath.isEmpty()) {
        this->originImage = QImage(imagePath);
        this->currentImage = this->originImage;

        QSize windowSize = this->size();
        QSize targetSize = windowSize * 0.7;

        qreal widthFactor = (qreal)targetSize.width() / (qreal)originImage.width();
        qreal heightFactor = (qreal)targetSize.height() / (qreal)originImage.height();
        qreal scaleFactor = qMin(widthFactor, heightFactor);

        scaleSlider->setValue(qRound(scaleFactor * 100));
    }
}

void ImageEditor::resetImage() {
    if (this->originImage.isNull()) {
        QMessageBox::warning(this, tr("警告"), tr("未打开图片"));
        return;
    }

    this->currentImage = this->originImage;
    this->setImage(this->currentImage);
}

void ImageEditor::setImage(const QImage& image) {
    if (image.isNull()) {
        QMessageBox::warning(this, tr("警告"), tr("图片不存在"));
        return;
    }

    this->currentImage = image;
    qreal scaleFactor = scaleSlider->value() / 100.0;
    QImage scaledImage = image.scaled(
        this->originImage.size() * scaleFactor,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    imageLabel->setPixmap(QPixmap::fromImage(scaledImage));
}

void ImageEditor::exportImage() {
    if (this->currentImage.isNull()) {
        QMessageBox::warning(this, tr("警告"), tr("没有可导出的图片"));
        return;
    }

    QString savePath = QFileDialog::getSaveFileName(
        this,
        tr("保存图片"),
        "",
        tr("PNG 文件 (*.png);;JPEG 文件 (*.jpg);;BMP 文件 (*.bmp)"));

    if (!savePath.isEmpty()) {
        // 自动补充文件扩展名
        QFileInfo fileInfo(savePath);
        QString suffix = fileInfo.suffix().toLower();
        if (suffix.isEmpty()) {
            savePath += ".png";  // 默认使用 PNG 格式
        }

        if (!this->currentImage.save(savePath)) {
            QMessageBox::warning(this, tr("错误"), tr("图片保存失败"));
        }
    }
}
