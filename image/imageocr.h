//
// Created by 37702 on 2025/6/5.
//

#ifndef IMAGEOCR_H
#define IMAGEOCR_H
#include <QImage>
#include <QString>

class ImageOCR {
public:
    static QString getText(QImage path);
};

#endif //IMAGEOCR_H
