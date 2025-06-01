#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMenu>
#include <QMessageBox>
#include <QTabWidget>
#include <QMenuBar>
#include "image/imageeditor.h"
#include "video/videoeditor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu* helpMenu = menuBar->addMenu(tr("帮助"));

    QAction* aboutAction = helpMenu->addAction(tr("关于"));
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, tr("关于"), tr("imageVideoEditorV1.0\n作者：李钺\n学号：249400220"));
    });

    QTabWidget* tabWidget = new QTabWidget(this);
    tabWidget->addTab(new ImageEditor(this), tr("图片编辑")); // 占位页面可替换为实际内容
    tabWidget->addTab(new VideoEditor(this), tr("视频编辑"));
    setCentralWidget(tabWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
