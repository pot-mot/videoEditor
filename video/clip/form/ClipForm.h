#ifndef CLIPFORM_H
#define CLIPFORM_H

#include <QLineEdit>
#include <QSpinBox>
#include <QWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>

#include "../ClipBase.h"
#include "../../effect/MatEffect.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class ClipForm;
}

QT_END_NAMESPACE

class ClipForm : public QWidget {
Q_OBJECT

public:
    explicit ClipForm(QWidget *parent = nullptr);
    ~ClipForm() override;

    void setClip(Clip* clip);

signals:
    void beforeClipChange();
    void clipChanged();

private slots:
    void onBrowseClicked();

    void onAddEffectClicked();
    void onRemoveEffectClicked(QListWidgetItem *item);

    void onSaveClicked();

private:
    void applyToClip();

    Ui::ClipForm *ui;
    Clip* currentClip = nullptr;

    QGridLayout* layout;

    QPushButton *browseButton;
    QPushButton *saveButton;

    QLineEdit* filePathEdit;
    QSpinBox* startTimeSpin;
    QSpinBox* offsetTimeSpin;
    QSpinBox* durationSpin;

    QWidget* displayAreaConfig;
    QSpinBox* displayAreaLeftSpin;
    QSpinBox* displayAreaTopSpin;
    QSpinBox* displayAreaHeightSpin;
    QSpinBox* displayAreaWidthSpin;

    QWidget* effectConfig;
    QComboBox *effectSelector;
    QListWidget *selectedEffectList;
    bool hasExclusiveEffect() const;
};

#endif // CLIPFORM_H