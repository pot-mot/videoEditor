//
// Created by 37702 on 2025/6/5.
//
#include "imageocr.h"
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include "ImageProcessor.h"
#include <QImage>

QString ImageOCR::getText(QImage image) {
    QImage grayImage = ImageProcessor::grayscaleImage(image);

    // 创建 PIX 对象
    PIX *pix = pixCreate(grayImage.width(), grayImage.height(), 8);
    for (int y = 0; y < grayImage.height(); ++y) {
        for (int x = 0; x < grayImage.width(); ++x) {
            QRgb pixel = grayImage.pixel(x, y);
            pixSetPixel(pix, x, y, qGray(pixel));
        }
    }

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "chi_sim")) {
        delete api;
        return "";
    }

    // 设置图像并执行 OCR
    api->SetImage(pix);
    char *outText = api->GetUTF8Text();

    // 清理资源
    delete[] outText;
    api->End();
    delete api;
    pixDestroy(&pix);

    return QString::fromUtf8(outText);

    return "";
}
