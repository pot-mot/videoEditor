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

    displayAreaLeftSpin = new QSpinBox(this);
    displayAreaLeftSpin->setRange(-100000, 100000);

    displayAreaTopSpin = new QSpinBox(this);
    displayAreaTopSpin->setRange(-100000, 100000);

    displayAreaWidthSpin = new QSpinBox(this);
    displayAreaWidthSpin->setRange(0, 10000);

    displayAreaHeightSpin = new QSpinBox(this);
    displayAreaHeightSpin->setRange(0, 10000);

    layout->addWidget(new QLabel("Display Area left"), row, 0);
    layout->addWidget(displayAreaLeftSpin, row++, 1);

    layout->addWidget(new QLabel("Display Area top"), row, 0);
    layout->addWidget(displayAreaTopSpin, row++, 1);

    layout->addWidget(new QLabel("Display Area width"), row, 0);
    layout->addWidget(displayAreaWidthSpin, row++, 1);

    layout->addWidget(new QLabel("Display Area height"), row, 0);
    layout->addWidget(displayAreaHeightSpin, row++, 1);

    effectList = new QListWidget(this);
    effectList->addItems(effectNameMap.keys());
    effectList->setSelectionMode(QAbstractItemView::MultiSelection);
    layout->addWidget(new QLabel("Effects"), row++, 0);
    layout->addWidget(effectList, row, 0, 1, 2);

    saveButton = new QPushButton("Save", this);
    connect(saveButton, &QPushButton::clicked, this, &ClipForm::onSaveClicked);
    layout->addWidget(saveButton);
}

ClipForm::~ClipForm() {
    delete ui;
}

void ClipForm::setClip(Clip *clip) {
    if (clip == currentClip) return;
    currentClip = clip;

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
        displayAreaLeftSpin->show();
        displayAreaTopSpin->show();
        displayAreaWidthSpin->show();
        displayAreaHeightSpin->show();

        displayAreaLeftSpin->setValue(displayArea.left());
        displayAreaTopSpin->setValue(displayArea.top());
        displayAreaWidthSpin->setValue(displayArea.width());
        displayAreaHeightSpin->setValue(displayArea.height());
    } else {
        displayAreaLeftSpin->hide();
        displayAreaTopSpin->hide();
        displayAreaWidthSpin->hide();
        displayAreaHeightSpin->hide();
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
        effectList->show();

        for (int i = 0; i < effectList->count(); ++i) {
            QString effectName = effectList->item(i)->text();
            bool isSelected = false;
            for (MatEffect* effect : externalEffect) {
                if (effect->type() == effectNameMap[effectName]) {
                    isSelected = true;
                    break;
                }
            }
            effectList->item(i)->setSelected(isSelected);
        }
    } else {
        effectList->hide();
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
