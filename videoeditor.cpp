#include "videoeditor.h"
#include "ui_videoeditor.h"

#include <QHBoxLayout>

VideoEditor::VideoEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VideoEditor)
{
    ui->setupUi(this);
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
    fileResourceTree = new QTreeWidget(this);
    fileResourceTree->setHeaderLabel("文件资源");
    leftLayout->addWidget(fileResourceTree);

    // 中间栏 (3)
    QWidget *middleContainer = new QWidget(this);
    QVBoxLayout *middleLayout = new QVBoxLayout(middleContainer);
    videoPreview = new QVideoWidget(this);
    mainTimeline = new QSlider(Qt::Horizontal, this);
    sliceTimeline = new QSlider(Qt::Horizontal, this);
    middleLayout->addWidget(videoPreview);
    middleLayout->addWidget(mainTimeline);
    middleLayout->addWidget(sliceTimeline);
    connect(mainTimeline, &QSlider::valueChanged, this, [this](int value) {
        qDebug() << "Main timeline position:" << value;
    });

    // 右侧栏 (1)
    QWidget *rightContainer = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightContainer);
    functionMenu = new QToolButton(this);
    functionMenu->setText("功能菜单");
    rightLayout->addWidget(functionMenu);
    connect(sliceTimeline, &QSlider::valueChanged, this, [this](int value) {
        qDebug() << "Slice timeline position:" << value;
    });
    connect(functionMenu, &QToolButton::clicked, this, []() {
        qDebug() << "Function menu clicked";
    });

    // 设置主布局
    mainLayout->addWidget(leftContainer, 1);
    mainLayout->addWidget(middleContainer, 3);
    mainLayout->addWidget(rightContainer, 1);
    setCentralWidget(centralWidget);
}
