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
    mainTimeline = new QSlider(Qt::Horizontal, this);
    mainTimeline->setRange(0, 0);
    connect(mainTimeline, &QSlider::valueChanged, this, [this](int value) {
        QImage result = ClipsPreview::preview(sliceTimeline->getClips(), value, videoPreview->width(), videoPreview->height(), fps);
        videoPreview->setPixmap(QPixmap::fromImage(result));
    });
    sliceTimeline = new VideoTimeline(this);
    connect(sliceTimeline, &VideoTimeline::totalDurationChange, this, [this](int duration) {
        mainTimeline->setMaximum(duration);
    });
    connect(sliceTimeline, &VideoTimeline::clipChanged, this, [this]() {
        QImage result = ClipsPreview::preview(sliceTimeline->getClips(), mainTimeline->value(), videoPreview->width(), videoPreview->height(), fps);
        videoPreview->setPixmap(QPixmap::fromImage(result));
    });
    QSpinBox* scaleSpinBox = new QSpinBox(this);
    scaleSpinBox->setRange(1, 100);
    scaleSpinBox->setValue(5);
    connect(scaleSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
        sliceTimeline->setScale(1.0 / value);
        sliceTimeline->update();
    });
    middleLayout->addWidget(videoPreview);
    middleLayout->addWidget(mainTimeline);
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
