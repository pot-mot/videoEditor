#include "fileviewer.h"
#include "ui_FileViewer.h"
#include <QDir>
#include <QFileInfo>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QDebug>

FileViewer::FileViewer(QWidget *parent) :
    QTreeWidget(parent), ui(new Ui::FileViewer) {
    ui->setupUi(this);

    selectDirButton = new QPushButton("选择文件夹", this);
    connect(selectDirButton, &QPushButton::clicked, this, &FileViewer::onSelectBaseDir);
}

FileViewer::~FileViewer() {
    delete ui;
}

// 新增方法：设置基础路径并加载文件树
void FileViewer::setBasePath(const QString &path) {
    clear(); // 清空现有内容
    QDir baseDir(path);
    if (!baseDir.exists()) {
        return;
    }

    // 遍历基础路径下的所有文件和目录
    loadDirectory(baseDir, nullptr, this->typeFilters);
}

// 新增方法：递归加载目录内容
void FileViewer::loadDirectory(const QDir &dir, QTreeWidgetItem *parentItem, const QStringList &filters) {
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for (const QFileInfo &entry : entries) {
        if (parentItem == nullptr) {
            QTreeWidgetItem *item = new QTreeWidgetItem(this, QStringList(entry.fileName()));
            if (entry.isDir()) {
                loadDirectory(QDir(entry.absoluteFilePath()), item, filters);
            }
        } else {
            QTreeWidgetItem *item = new QTreeWidgetItem(parentItem, QStringList(entry.fileName()));
            if (entry.isDir()) {
                loadDirectory(QDir(entry.absoluteFilePath()), item, filters);
            }
        }
    }
}

// 新增方法：根据文件名关键字搜索文件
void FileViewer::searchFiles(const QString &keyword) {
    clear(); // 清空现有内容
    QTreeWidgetItemIterator it(this);
    while (*it) {
        if ((*it)->text(0).contains(keyword, Qt::CaseInsensitive)) {
            (*it)->setHidden(false);
        } else {
            (*it)->setHidden(true);
        }
        ++it;
    }
}

// 新增方法：打开文件夹选择对话框并设置基础路径
void FileViewer::onSelectBaseDir() {
    QString dirPath = QFileDialog::getExistingDirectory(this, "选择文件夹", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dirPath.isEmpty()) {
        setBasePath(dirPath); // 调用 setBasePath 设置基础路径
    }
}