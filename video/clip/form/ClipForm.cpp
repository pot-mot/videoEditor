#include "ClipForm.h"
#include "ui_ClipForm.h"

#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "../ImageClip.h"
#include "../VideoClip.h"

ClipForm::ClipForm(QWidget *parent): QWidget(parent), ui(new Ui::ClipForm) {
    ui->setupUi(this);

    layout = new QGridLayout(this);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 3);
    setLayout(layout);

    baseConfig = new QWidget(this);
    QGridLayout *baseConfigLayout = new QGridLayout(baseConfig);
    baseConfig->setLayout(baseConfigLayout);
    baseConfigLayout->setColumnStretch(0, 1);
    baseConfigLayout->setColumnStretch(1, 3);

    filePathEdit = new QLineEdit(baseConfig);
    browseButton = new QPushButton("Browse", baseConfig);
    connect(browseButton, &QPushButton::clicked, this, &ClipForm::onBrowseClicked);

    baseConfigLayout->addWidget(filePathEdit, 0, 0, 1, 2);
    baseConfigLayout->addWidget(browseButton, 1, 0);

    startTimeSpin = new QSpinBox(baseConfig);
    startTimeSpin->setRange(-1000000, 1000000);

    offsetTimeSpin = new QSpinBox(baseConfig);
    offsetTimeSpin->setRange(0, 1000000);

    durationSpin = new QSpinBox(baseConfig);
    durationSpin->setRange(0, 1000000);

    baseConfigLayout->addWidget(new QLabel("Start Time"), 2, 0);
    baseConfigLayout->addWidget(startTimeSpin, 2, 1);

    baseConfigLayout->addWidget(new QLabel("Offset Time"), 3, 0);
    baseConfigLayout->addWidget(offsetTimeSpin, 3, 1);

    baseConfigLayout->addWidget(new QLabel("Duration"), 4, 0);
    baseConfigLayout->addWidget(durationSpin, 4, 1);

    layout->addWidget(baseConfig, 2, 0, 1, 2);
    baseConfig->hide();

    displayAreaConfig = new QWidget(this);
    QGridLayout *displayAreaConfigLayout = new QGridLayout(displayAreaConfig);
    displayAreaConfig->setLayout(displayAreaConfigLayout);
    displayAreaConfigLayout->setColumnStretch(0, 1);
    displayAreaConfigLayout->setColumnStretch(1, 3);

    displayAreaLeftSpin = new QSpinBox(displayAreaConfig);
    displayAreaLeftSpin->setRange(-100000, 100000);

    displayAreaTopSpin = new QSpinBox(displayAreaConfig);
    displayAreaTopSpin->setRange(-100000, 100000);

    displayAreaWidthSpin = new QSpinBox(displayAreaConfig);
    displayAreaWidthSpin->setRange(0, 10000);

    displayAreaHeightSpin = new QSpinBox(displayAreaConfig);
    displayAreaHeightSpin->setRange(0, 10000);

    displayAreaConfigLayout->addWidget(new QLabel("Display Area left"), 0, 0);
    displayAreaConfigLayout->addWidget(displayAreaLeftSpin, 0, 1);

    displayAreaConfigLayout->addWidget(new QLabel("Display Area top"), 1, 0);
    displayAreaConfigLayout->addWidget(displayAreaTopSpin, 1, 1);

    displayAreaConfigLayout->addWidget(new QLabel("Display Area width"), 2, 0);
    displayAreaConfigLayout->addWidget(displayAreaWidthSpin, 2, 1);

    displayAreaConfigLayout->addWidget(new QLabel("Display Area height"), 3, 0);
    displayAreaConfigLayout->addWidget(displayAreaHeightSpin, 3, 1);

    layout->addWidget(displayAreaConfig, 3, 0, 1, 2);
    displayAreaConfig->hide();


    effectConfig = new QWidget(this);
    QGridLayout *effectConfigLayout = new QGridLayout(effectConfig);
    effectConfig->setLayout(effectConfigLayout);

    effectSelector = new QComboBox(effectConfig);
    effectSelector->addItems(effectNameMap.keys());

    selectedEffectList = new QListWidget(effectConfig);

    QPushButton *addButton = new QPushButton("Add Effect", effectConfig);
    connect(addButton, &QPushButton::clicked, this, &ClipForm::onAddEffectClicked);

    connect(selectedEffectList, &QListWidget::itemClicked, this, &ClipForm::onRemoveEffectClicked);

    effectConfigLayout->addWidget(new QLabel("Available Effects"), 0, 0);
    effectConfigLayout->addWidget(effectSelector, 1, 0);
    effectConfigLayout->addWidget(addButton, 2, 0);
    effectConfigLayout->addWidget(new QLabel("Selected Effects"), 3, 0);
    effectConfigLayout->addWidget(selectedEffectList, 4, 0);

    layout->addWidget(effectConfig, 4, 0, 1, 2);
    effectConfig->hide();

    saveButton = new QPushButton("Save", this);
    connect(saveButton, &QPushButton::clicked, this, &ClipForm::onSaveClicked);
    layout->addWidget(saveButton, 5, 0, 1, 2);
    saveButton->hide();
}

ClipForm::~ClipForm() {
    delete ui;
}

void ClipForm::setClip(Clip *clip) {
    currentClip = clip;
    if (currentClip == nullptr) {
        baseConfig->hide();

        displayAreaConfig->hide();

        effectConfig->hide();

        saveButton->hide();
    } else {
        filePathEdit->setText(currentClip->getFilePath());

        startTimeSpin->setValue(currentClip->getStartTime());
        offsetTimeSpin->setValue(currentClip->getOffsetTime());
        durationSpin->setValue(currentClip->getDuration());

        baseConfig->show();

        saveButton->show();

        bool hasDisplayArea = false;
        cv::Rect displayArea;

        if (currentClip->getType() == ResourceType::Video) {
            displayArea = dynamic_cast<VideoClip *>(currentClip)->getDisplayArea();
            hasDisplayArea = true;
        } else if (currentClip->getType() == ResourceType::Image) {
            displayArea = dynamic_cast<ImageClip *>(currentClip)->getDisplayArea();
            hasDisplayArea = true;
        }

        if (hasDisplayArea) {
            displayAreaConfig->show();

            displayAreaLeftSpin->setValue(displayArea.x);
            displayAreaTopSpin->setValue(displayArea.y);
            displayAreaWidthSpin->setValue(displayArea.width);
            displayAreaHeightSpin->setValue(displayArea.height);
        } else {
            displayAreaConfig->hide();
        }

        bool hasExternalEffect = false;
        QList<MatEffect *> externalEffect;

        if (currentClip->getType() == ResourceType::Video) {
            externalEffect = dynamic_cast<VideoClip *>(currentClip)->getExternalEffect();
            hasExternalEffect = true;
        } else if (currentClip->getType() == ResourceType::Image) {
            externalEffect = dynamic_cast<ImageClip *>(currentClip)->getExternalEffect();
            hasExternalEffect = true;
        }

        if (hasExternalEffect) {
            if (hasExternalEffect) {
                effectConfig->show();

                selectedEffectList->clear();
                for (MatEffect *effect : externalEffect) {
                    QString effectName = effectTypeMap[effect->type()];
                    selectedEffectList->addItem(effectName);
                }
            }
        } else {
            effectConfig->hide();
        }
    }
}

void ClipForm::onBrowseClicked() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select File");
    if (!filePath.isEmpty()) {
        filePathEdit->setText(filePath);
    }
}

bool ClipForm::hasExclusiveEffect() const {
    for (int i = 0; i < selectedEffectList->count(); ++i) {
        QString effectName = selectedEffectList->item(i)->text();
        MatEffect *effect = EffectFactory::createEffect(effectNameMap[effectName]);
        if (effect && effect->isExclusive()) {
            delete effect;
            return true;
        }
    }
    return false;
}

void ClipForm::onAddEffectClicked() {
    QString selectedEffect = effectSelector->currentText();
    if (!selectedEffect.isEmpty()) {
        MatEffect *effect = EffectFactory::createEffect(effectNameMap[selectedEffect]);
        if (effect != nullptr) {
            if (effect->isExclusive() || hasExclusiveEffect()) {
                // 如果是排斥效果或者当前具有排斥效果，清空当前列表
                selectedEffectList->clear();
            }
            selectedEffectList->addItem(selectedEffect);
        }
    }
}

void ClipForm::onRemoveEffectClicked(QListWidgetItem *item) {
    delete selectedEffectList->takeItem(selectedEffectList->row(item));
}

void ClipForm::onSaveClicked() {
    if (currentClip) {
        applyToClip();
    }
}

void ClipForm::applyToClip() {
    if (currentClip == nullptr) return;

    emit beforeClipChange();

    currentClip->setFilePath(filePathEdit->text());
    currentClip->setStartTime(startTimeSpin->value());
    currentClip->setOffsetTime(offsetTimeSpin->value());
    currentClip->setDuration(durationSpin->value());

    bool hasDisplayArea = false;
    bool hasExternalEffect = false;
    if (currentClip->getType() == ResourceType::Video) {
        hasDisplayArea = true;
        hasExternalEffect = true;
    } else if (currentClip->getType() == ResourceType::Image) {
        hasDisplayArea = true;
        hasExternalEffect = true;
    }

    if (hasDisplayArea) {
        cv::Rect displayArea(
            displayAreaLeftSpin->value(),
            displayAreaTopSpin->value(),
            displayAreaWidthSpin->value(),
            displayAreaHeightSpin->value()
        );

        if (currentClip->getType() == ResourceType::Video) {
            dynamic_cast<VideoClip *>(currentClip)->setDisplayArea(displayArea);
        } else if (currentClip->getType() == ResourceType::Image) {
            dynamic_cast<ImageClip *>(currentClip)->setDisplayArea(displayArea);
        }
    }

    if (hasExternalEffect) {
        QList<MatEffect *> effects;
        for (int i = 0; i < selectedEffectList->count(); ++i) {
            QString effectName = selectedEffectList->item(i)->text();
            effects.append(EffectFactory::createEffect(effectNameMap[effectName]));
        }

        if (currentClip->getType() == ResourceType::Video) {
            dynamic_cast<VideoClip *>(currentClip)->setExternalEffect(effects);
        } else if (currentClip->getType() == ResourceType::Image) {
            dynamic_cast<ImageClip *>(currentClip)->setExternalEffect(effects);
        }
    }

    emit clipChanged();
}
