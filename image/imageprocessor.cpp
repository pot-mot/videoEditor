#include "imageprocessor.h"

ImageProcessor::ImageProcessor() {}

QImage ImageProcessor::grayscaleImage(const QImage& baseImage) {
    QImage image = baseImage.copy();
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = QColor(image.pixel(x, y));
            int gray = (color.red() + color.green() + color.blue()) / 3;
            image.setPixel(x, y, QColor(gray, gray, gray).rgb());
        }
    }
    return image;
}

QImage ImageProcessor::binarizeImage(const QImage& baseImage, const int threshold) {
    QImage image = baseImage.copy();
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = QColor(image.pixel(x, y));
            int gray = (color.red() + color.green() + color.blue()) / 3;
            if (gray > threshold) {
                image.setPixel(x, y, QColor(255, 255, 255).rgb());
            } else {
                image.setPixel(x, y, QColor(0, 0, 0).rgb());
            }
        }
    }
    return image;
}

QImage ImageProcessor::meanFilterImage(const QImage& baseImage) {
    QImage image = baseImage.copy();
    for (int y = 1; y < image.height() - 1; ++y) {
        for (int x = 1; x < image.width() - 1; ++x) {
            int rSum = 0, gSum = 0, bSum = 0;
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    QColor color = QColor(image.pixel(x + i, y + j));
                    rSum += color.red();
                    gSum += color.green();
                    bSum += color.blue();
                }
            }
            QColor avgColor(rSum / 9, gSum / 9, bSum / 9);
            image.setPixel(x, y, avgColor.rgb());
        }
    }
    return image;
}

QImage ImageProcessor::gammaCorrection(const QImage& baseImage) {
    QImage image = baseImage.copy();
    double gamma = 2.2; // 伽马值
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = QColor(image.pixel(x, y));
            int r = static_cast<int>(pow(color.red() / 255.0, gamma) * 255);
            int g = static_cast<int>(pow(color.green() / 255.0, gamma) * 255);
            int b = static_cast<int>(pow(color.blue() / 255.0, gamma) * 255);
            QColor correctedColor(r, g, b);
            image.setPixel(x, y, correctedColor.rgb());
        }
    }
    return image;
}

QImage ImageProcessor::edgeDetection(const QImage& baseImage) {
    QImage grayImage = grayscaleImage(baseImage);
    QImage edgeImage(grayImage.size(), QImage::Format_ARGB32);

    // Sobel算子核
    int sobelX[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    int sobelY[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int y = 1; y < grayImage.height() - 1; ++y) {
        for (int x = 1; x < grayImage.width() - 1; ++x) {
            int gx = 0, gy = 0;

            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    int pixel = qGray(grayImage.pixel(x + i, y + j));
                    gx += pixel * sobelX[i + 1][j + 1];
                    gy += pixel * sobelY[i + 1][j + 1];
                }
            }

            int magnitude = static_cast<int>(std::sqrt(gx * gx + gy * gy));
            magnitude = std::min(magnitude, 255);

            edgeImage.setPixel(x, y, qRgb(magnitude, magnitude, magnitude));
        }
    }

    return edgeImage;
}
