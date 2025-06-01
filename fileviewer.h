#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QDir>
#include <QTreeWidget>
#include <QStringList>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class FileViewer; }
QT_END_NAMESPACE

class FileViewer : public QTreeWidget {
    Q_OBJECT

public:
    explicit FileViewer(QWidget *parent = nullptr);
    ~FileViewer() override;

    QStringList typeFilters;

    // 设置基础路径并加载文件树
    void setBasePath(const QString &path);

    // 根据文件名关键字搜索文件
    void searchFiles(const QString &keyword);

private:
    Ui::FileViewer *ui;
    QPushButton *selectDirButton;

    // 递归加载目录内容
    void loadDirectory(const QDir &dir, QTreeWidgetItem *parentItem, const QStringList &filters);

private slots:
    void onSelectBaseDir(); // 新增：定义槽函数
};

#endif // FILEVIEWER_H