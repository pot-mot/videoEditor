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

    int row = 0;

    filePathEdit = new QLineEdit(this);
    browseButton = new QPushButton("Browse", this);
    connect(browseButton, &QPushButton::clicked, this, &ClipForm::onBrowseClicked);

    startTimeSpin = new QSpinBox(this);
    startTimeSpin->setRange(0, 1000000);

    offsetTimeSpin = new QSpinBox(this);
    offsetTimeSpin->setRange(0, 1000000);

    durationSpin = new QSpinBox(this);
    durationSpin->setRange(0, 1000000);

    layout->addWidget(filePathEdit, row++, 0, 1, 2);
    layout->addWidget(browseButton, row++, 0);

    layout->addWidget(new QLabel("Start Time"), row, 0);
    layout->addWidget(startTimeSpin, row++, 1);

    layout->addWidget(new QLabel("Offset Time"), row, 0);
    layout->addWidget(offsetTimeSpin, row++, 1);

    layout->addWidget(new QLabel("Duration"), row, 0);
    layout->addWidget(durationSpin, row++, 1);

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

    layout->addWidget(displayAreaConfig, row++, 0, 1, 2);
    displayAreaConfig->hide();


    effectConfig = new QWidget(this);
    QGridLayout *effectConfigLayout = new QGridLayout(effectConfig);
    effectConfig->setLayout(effectConfigLayout);

    effectList = new QListWidget(effectConfig);
    effectList->addItems(effectNameMap.keys());
    effectList->setSelectionMode(QAbstractItemView::MultiSelection);
    effectConfigLayout->addWidget(new QLabel("Effects"), 0, 0);
    effectConfigLayout->addWidget(effectList, 1, 0, 1, 2);

    layout->addWidget(effectConfig, row, 0, 1, 2);
    effectConfig->hide();

    saveButton = new QPushButton("Save", this);
    connect(saveButton, &QPushButton::clicked, this, &ClipForm::onSaveClicked);
    layout->addWidget(saveButton);
}

ClipForm::~ClipForm() {
    delete ui;
}

void ClipForm::setClip(Clip *clip) {
    currentClip = clip;
    if (currentClip == nullptr) {
        filePathEdit->setText("");

        startTimeSpin->setValue(0);
        offsetTimeSpin->setValue(0);
        durationSpin->setValue(0);

        displayAreaConfig->hide();

        effectConfig->hide();
    } else {
        filePathEdit->setText(currentClip->getFilePath());

        startTimeSpin->setValue(currentClip->getStartTime());
        offsetTimeSpin->setValue(currentClip->getOffsetTime());
        durationSpin->setValue(currentClip->getDuration());

        bool hasDisplayArea = false;
        QRect displayArea;

        if (currentClip->getType() == ResourceType::Video) {
            displayArea = dynamic_cast<VideoClip *>(currentClip)->getDisplayArea();
            hasDisplayArea = true;
        } else if (currentClip->getType() == ResourceType::Image) {
            displayArea = dynamic_cast<ImageClip *>(currentClip)->getDisplayArea();
            hasDisplayArea = true;
        }

        if (hasDisplayArea) {
            displayAreaConfig->show();

            displayAreaLeftSpin->setValue(displayArea.left());
            displayAreaTopSpin->setValue(displayArea.top());
            displayAreaWidthSpin->setValue(displayArea.width());
            displayAreaHeightSpin->setValue(displayArea.height());
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
            effectConfig->show();

            for (int i = 0; i < effectList->count(); ++i) {
                QString effectName = effectList->item(i)->text();
                bool isSelected = false;
                for (MatEffect *effect: externalEffect) {
                    if (effect->type() == effectNameMap[effectName]) {
                        isSelected = true;
                        break;
                    }
                }
                effectList->item(i)->setSelected(isSelected);
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
        QRect displayArea(
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
        for (int i = 0; i < effectList->count(); ++i) {
            if (effectList->item(i)->isSelected()) {
                QString effectName = effectList->item(i)->text();
                effects.append(EffectFactory::createEffect(effectNameMap[effectName]));
            }
        }
        if (currentClip->getType() == ResourceType::Video) {
            dynamic_cast<VideoClip *>(currentClip)->setExternalEffect(effects);
        } else if (currentClip->getType() == ResourceType::Image) {
            dynamic_cast<ImageClip *>(currentClip)->setExternalEffect(effects);
        }
    }

    emit clipChanged();
}
