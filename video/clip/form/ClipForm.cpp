#include "ClipForm.h"
#include "ui_ClipForm.h"

#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

ClipForm::ClipForm(QWidget *parent): QWidget(parent), ui(new Ui::ClipForm) {
    ui->setupUi(this);

    filePathEdit = new QLineEdit(this);
    QPushButton *browseButton = new QPushButton("Browse", this);
    startTimeSpin = new QSpinBox(this);
    offsetTimeSpin = new QSpinBox(this);
    durationSpin = new QSpinBox(this);
    effectList = new QListWidget(this);
    QPushButton *saveButton = new QPushButton("Save", this);

    startTimeSpin->setRange(0, 1000000);
    offsetTimeSpin->setRange(0, 1000000);
    durationSpin->setRange(0, 1000000);

    effectList->addItems({"Grayscale", "Binarize", "Mean Filter", "Gamma Correction", "Edge Detection"});
    effectList->setSelectionMode(QAbstractItemView::MultiSelection);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(filePathEdit);
    layout->addWidget(browseButton);
    layout->addWidget(new QLabel("Start Time"));
    layout->addWidget(startTimeSpin);
    layout->addWidget(new QLabel("Offset Time"));
    layout->addWidget(offsetTimeSpin);
    layout->addWidget(new QLabel("Duration"));
    layout->addWidget(durationSpin);
    layout->addWidget(new QLabel("Effects"));
    layout->addWidget(effectList);
    layout->addWidget(saveButton);
    setLayout(layout);

    connect(browseButton, &QPushButton::clicked, this, &ClipForm::onBrowseClicked);
    connect(saveButton, &QPushButton::clicked, this, &ClipForm::onSaveClicked);
}

ClipForm::~ClipForm() {
    delete ui;
}

void ClipForm::setClip(Clip *clip) {
    currentClip = clip;
    if (currentClip != nullptr) {
        filePathEdit->setText(currentClip->getFilePath());
        startTimeSpin->setValue(currentClip->getStartTime());
        offsetTimeSpin->setValue(currentClip->getOffsetTime());
        durationSpin->setValue(currentClip->getDuration());
    } else {
        filePathEdit->clear();
        startTimeSpin->setValue(0);
        offsetTimeSpin->setValue(0);
        durationSpin->setValue(0);
        effectList->clearSelection();
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

    QList<MatEffect *> effects;
    for (int i = 0; i < effectList->count(); ++i) {
        if (effectList->item(i)->isSelected()) {
            QString effectName = effectList->item(i)->text();
            if (effectName == "Grayscale") {
                effects.append(new GrayscaleEffect());
            } else if (effectName == "Binarize") {
                effects.append(new BinarizeEffect(128));
            } else if (effectName == "Mean Filter") {
                effects.append(new MeanFilterEffect());
            } else if (effectName == "Gamma Correction") {
                effects.append(new GammaCorrectionEffect(1.0));
            } else if (effectName == "Edge Detection") {
                effects.append(new EdgeDetectionEffect());
            }
        }
    }

    emit clipChanged();
}
