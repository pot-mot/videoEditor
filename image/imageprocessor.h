#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <QImage>

class ImageProcessor
{
public:
    ImageProcessor();

    static QImage grayscaleImage(const QImage& image);
    static QImage binarizeImage(const QImage& image, const int threshold);
    static QImage meanFilterImage(const QImage& image);
    static QImage gammaCorrection(const QImage& image);
    static QImage edgeDetection(const QImage& image);
};

#endif // IMAGEPROCESSOR_H
