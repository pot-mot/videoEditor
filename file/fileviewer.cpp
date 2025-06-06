#include "fileviewer.h"
#include "ui_fileviewer.h"
#include <QDir>
#include <QFileInfo>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QDebug>

FileViewer::FileViewer(QWidget *parent) :
    QWidget(parent), ui(new Ui::FileViewer) {
    ui->setupUi(this);

    // 创建 QVBoxLayout 布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    selectDirButton = new QPushButton("选择文件夹", this);
    connect(selectDirButton, &QPushButton::clicked, this, &FileViewer::onSelectBaseDir);
    layout->addWidget(selectDirButton); // 将按钮添加到布局

    filterLineEdit = new QLineEdit(this);
    connect(filterLineEdit, &QLineEdit::textChanged, this, &FileViewer::onFilterTextChanged);
    layout->addWidget(filterLineEdit); // 将输入框添加到布局

    treeWidget = new QTreeWidget(this);
    treeWidget->setMinimumWidth(150);
    treeWidget->setHeaderLabel("文件列表");
    connect(treeWidget, &QTreeWidget::itemClicked, this, &FileViewer::onTreeWidgetItemClicked);
    layout->addWidget(treeWidget); // 将树形控件添加到布局

    // 设置布局
    this->setLayout(layout);

    // 移除父组件的 padding
    this->setStyleSheet("FileViewer { padding: 0; }");
}

FileViewer::~FileViewer() {
    delete ui;
}

// 设置基础路径并加载文件树
void FileViewer::setBasePath(const QString &path) {
    treeWidget->clear(); // 清空现有内容
    QDir baseDir(path);
    if (!baseDir.exists()) {
        return;
    }

    // 遍历基础路径下的所有文件和目录
    loadDirectory(baseDir, nullptr, this->typeFilters);
}

// 递归加载目录内容
void FileViewer::loadDirectory(const QDir &dir, QTreeWidgetItem *parentItem, const QStringList &filters) {
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for (const QFileInfo &entry : entries) {
        if (parentItem == nullptr) {
            if (entry.isDir()) {
                QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget, QStringList(entry.fileName()));
                item->setData(0, Qt::UserRole, entry.absoluteFilePath());
                loadDirectory(QDir(entry.absoluteFilePath()), item, filters);
            } else if (entry.isFile() && filters.contains(entry.suffix())) {
                QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget, QStringList(entry.fileName()));
                item->setData(0, Qt::UserRole, entry.absoluteFilePath());
            }
        } else {
            if (entry.isDir()) {
                QTreeWidgetItem *item = new QTreeWidgetItem(parentItem, QStringList(entry.fileName()));
                item->setData(0, Qt::UserRole, entry.absoluteFilePath());
                loadDirectory(QDir(entry.absoluteFilePath()), item, filters);
            } else if (entry.isFile() && filters.contains(entry.suffix())) {
                QTreeWidgetItem *item = new QTreeWidgetItem(parentItem, QStringList(entry.fileName()));
                item->setData(0, Qt::UserRole, entry.absoluteFilePath());
            }
        }
    }
}

// 根据文件名关键字搜索文件
void FileViewer::searchFiles(const QString &keyword) {
    treeWidget->clear(); // 清空现有内容
    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        if ((*it)->text(0).contains(keyword, Qt::CaseInsensitive)) {
            (*it)->setHidden(false);
        } else {
            (*it)->setHidden(true);
        }
        ++it;
    }
}

// 打开文件夹选择对话框并设置基础路径
void FileViewer::onSelectBaseDir() {
    QString dirPath = QFileDialog::getExistingDirectory(this, "选择文件夹", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dirPath.isEmpty()) {
        setBasePath(dirPath); // 调用 setBasePath 设置基础路径
    }
}

// 根据输入框内容过滤文件
void FileViewer::onFilterTextChanged(const QString &text) {
    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        if ((*it)->text(0).contains(text, Qt::CaseInsensitive)) {
            (*it)->setHidden(false);
        } else {
            (*it)->setHidden(true);
        }
        ++it;
    }
}

void FileViewer::onTreeWidgetItemClicked(QTreeWidgetItem *item, int column) {
    Q_UNUSED(column); // 忽略列参数

    if (item && !item->childCount()) { // 确保是叶子节点
        QString filePath = item->data(0, Qt::UserRole).toString(); // 获取文件路径
        QFileInfo fileInfo(filePath);
        QString suffix = fileInfo.suffix();

        if (!filePath.isEmpty() && this->typeFilters.contains(suffix)) {
            emit fileSelected(filePath); // 发射信号
        }
    }
}