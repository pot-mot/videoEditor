#include "videoeditor.h"
#include "ui_videoeditor.h"

#include <QHBoxLayout>
#include <QFileDialog>

#include "preview/ClipsPreview.h"
#include "render/VideoRender.h"
#include "clip/AudioClip.h"
#include "clip/VideoClip.h"
#include "clip/ImageClip.h"
#include "clip/TextClip.h"
#include <QToolButton>
#include <QMediaPlayer>
#include <QSpinBox>
#include <QComboBox>

VideoEditor::VideoEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VideoEditor)
{
    ui->setupUi(this);
    fps = 30;
    width = 1280;
    height = 720;
    initUI(); // 调用初始化函数
}

VideoEditor::~VideoEditor()
{
    delete ui;
}

// 新增 initUI 函数
void VideoEditor::initUI()
{
    // 创建主布局
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // 左侧栏 (1)
    QWidget *leftContainer = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftContainer);
    fileResourceTree = new FileViewer(this);
    fileResourceTree->typeFilters = { "mp4", "png", "jpg", "mp3" };
    connect( fileResourceTree, &FileViewer::fileSelected, this, [this](const QString &filePath) {
        QFileInfo fileInfo(filePath);
        QString suffix = fileInfo.suffix().toLower();

        if (suffix == "mp4" || suffix == "mp3") {
            QMediaPlayer* player = new QMediaPlayer(this);
            connect(player, &QMediaPlayer::durationChanged, this, [this, filePath, suffix, player](int duration) {
                try {
                    Clip* clip = nullptr;
                    if (suffix == "mp4") {
                        clip = new VideoClip(filePath, 0, 0, duration, duration, QRect(0,0,width,height), "");
                    } else if (suffix == "mp3") {
                        clip = new AudioClip(filePath, 0, 0, duration, duration, 1.0, "");
                    }
                    qDebug() << "Added clip:" << filePath << "duration:" << duration;
                    if (clip != nullptr) {
                        sliceTimeline->addClip(clip);
                    }
                    player->deleteLater();
                } catch (const std::exception& e) {
                    player->deleteLater();
                }
            });
            player->setSource(QUrl::fromLocalFile(filePath));
        } else if (suffix == "png" || suffix == "jpg") {
            Clip* clip = new ImageClip(filePath, 0, 0, 5000, QRect(0,0,width,height), "");
            if (clip != nullptr) {
                qDebug() << "Added clip:" << filePath;
                sliceTimeline->addClip(clip);
            }
        }
    });
    leftLayout->addWidget(fileResourceTree);

    // 中间栏 (3)
    QWidget *middleContainer = new QWidget(this);
    QVBoxLayout *middleLayout = new QVBoxLayout(middleContainer);

    videoPreview = new QLabel(this);
    videoPreview->setMinimumWidth(320);
    videoPreview->setMinimumHeight(200);
    videoPreview->setAlignment(Qt::AlignCenter);

    // 创建时间轴与控制按钮的水平布局
    QHBoxLayout *mainTimelineLayout = new QHBoxLayout();
    mainTimelineLayout->setSpacing(5); // 设置控件间距

    // 时间轴滑块
    mainTimeline = new QSlider(Qt::Horizontal, this);
    mainTimeline->setRange(0, 0);
    mainTimeline->setMinimumWidth(60);
    connect(mainTimeline, &QSlider::valueChanged, this, [this]() {
        this->preview();
    });
    mainTimelineLayout->addWidget(mainTimeline);

    // 播放倍速
    QComboBox* speedComboBox = new QComboBox(this);
    speedComboBox->addItem("0.125x", "0.125");
    speedComboBox->addItem("0.25x",  "0.25");
    speedComboBox->addItem("0.5x",   "0.5");
    speedComboBox->addItem("0.75x",     "0.75");
    speedComboBox->addItem("1x",     "1.0");
    speedComboBox->addItem("1.25x",  "1.25");
    speedComboBox->addItem("1.5x",   "1.5");
    speedComboBox->addItem("2x",     "2.0");
    speedComboBox->addItem("4x",     "4.0");
    speedComboBox->addItem("8x",     "8.0");
    speedComboBox->setCurrentText("1x");

    connect(speedComboBox, &QComboBox::currentIndexChanged, this, [this, speedComboBox](int index) {
        playSpeed = speedComboBox->itemData(index).toString().toDouble();
    });

    mainTimelineLayout->addWidget(speedComboBox);

    // 播放/暂停按钮
    QPushButton *playPauseButton = new QPushButton(" > ", this);
    playPauseButton->setFixedWidth(32);
    connect(playPauseButton, &QPushButton::clicked, this, [this, playPauseButton]() {
        if (this->isPlaying) {
            playPauseButton->setText(" > ");
            playTimer->stop();
        } else {
            playPauseButton->setText("| |");
            playTimer->start();
        }
        isPlaying = !isPlaying;
    });

    double interval = 1000 / fps;
    playTimer->setInterval(interval);
    connect(playTimer, &QTimer::timeout, this, [this, playPauseButton, interval]() {
        int currentValue = mainTimeline->value();
        int maxValue = mainTimeline->maximum();

        if (currentValue < maxValue) {
            mainTimeline->setValue(static_cast<int>(currentValue + interval * this -> playSpeed));
        } else {
            playTimer->stop();
            isPlaying = false;
            playPauseButton->setText(" > ");
        }
    });

    // 后退
    QPushButton *rewindButton = new QPushButton("<<", this);
    rewindButton->setFixedWidth(32);
    connect(rewindButton, &QPushButton::clicked, this, [this]() {
        int newValue = qMax(0, mainTimeline->value() - 200);
        mainTimeline->setValue(newValue);
    });

    // 前进
    QPushButton *forwardButton = new QPushButton(">>", this);
    forwardButton->setFixedWidth(32);
    connect(forwardButton, &QPushButton::clicked, this, [this]() {
        int newValue = qMin(mainTimeline->maximum(), mainTimeline->value() + 200);
        mainTimeline->setValue(newValue);
    });

    mainTimelineLayout->addWidget(rewindButton);
    mainTimelineLayout->addWidget(playPauseButton);
    mainTimelineLayout->addWidget(forwardButton);

    sliceTimeline = new VideoTimeline(this);
    sliceTimeline->setMinimumHeight(120);
    connect(sliceTimeline, &VideoTimeline::totalDurationChange, this, [this](int duration) {
        mainTimeline->setMaximum(duration);
    });
    connect(sliceTimeline, &VideoTimeline::clipChanged, this, [this]() {
        this->preview();
    });
    QSpinBox* scaleSpinBox = new QSpinBox(this);
    scaleSpinBox->setRange(1, 100);
    scaleSpinBox->setValue(5);
    connect(scaleSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
        sliceTimeline->setScale(1.0 / value);
        sliceTimeline->update();
    });
    middleLayout->addWidget(videoPreview);
    middleLayout->addLayout(mainTimelineLayout);
    middleLayout->addWidget(scaleSpinBox);
    middleLayout->addWidget(sliceTimeline);

    // 右侧栏 (1)
    QWidget *rightContainer = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightContainer);

    QToolButton* exportButton = new QToolButton(this);
    exportButton->setText("导出");
    connect(exportButton, &QToolButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getSaveFileName(this, "保存视频文件", "", "MP4 Files (*.mp4)");
        if (!filePath.isEmpty()) {
            QList<Clip*> clips = sliceTimeline->getClips();
            VideoRender::render(filePath.toStdString(), clips, width, height, fps);
        }
    });
    rightLayout->addWidget(exportButton);

    // 设置主布局
    mainLayout->addWidget(leftContainer, 1);
    mainLayout->addWidget(middleContainer, 3);
    mainLayout->addWidget(rightContainer, 1);
    setCentralWidget(centralWidget);
}

void VideoEditor::resizeEvent(QResizeEvent *event) {
    this->preview();
}

void VideoEditor::preview() {
    QImage result = ClipsPreview::preview(sliceTimeline->getClips(), mainTimeline->value(), width, height, fps);
    QPixmap pixmap = QPixmap::fromImage(result);
    videoPreview->setPixmap(pixmap.scaled(videoPreview->width(), videoPreview->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
