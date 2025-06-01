#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QWidget>
#include <QDir>
#include <QTreeWidget>
#include <QStringList>
#include <QPushButton>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class FileViewer; }
QT_END_NAMESPACE

class FileViewer : public QWidget {
    Q_OBJECT

public:
    explicit FileViewer(QWidget *parent = nullptr);
    ~FileViewer() override;

    QStringList typeFilters;

    // 设置基础路径并加载文件树
    void setBasePath(const QString &path);

    // 根据文件名关键字搜索文件
    void searchFiles(const QString &keyword);

signals:
    void fileSelected(const QString &filePath);


private:
    Ui::FileViewer *ui;
    QPushButton *selectDirButton;
    QLineEdit *filterLineEdit;
    QTreeWidget *treeWidget; // 新增：将 QTreeWidget 作为私有属性

    // 递归加载目录内容
    void loadDirectory(const QDir &dir, QTreeWidgetItem *parentItem, const QStringList &filters);

private slots:
    void onSelectBaseDir();

    void onFilterTextChanged(const QString &text);

    void onTreeWidgetItemClicked(QTreeWidgetItem *item, int column);
};

#endif // FILEVIEWER_H