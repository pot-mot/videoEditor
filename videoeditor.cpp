#include "videoeditor.h"
#include "ui_videoeditor.h"

VideoEditor::VideoEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoEditor)
{
    ui->setupUi(this);
}

VideoEditor::~VideoEditor()
{
    delete ui;
}
