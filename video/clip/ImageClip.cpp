#include <opencv2/opencv.hpp>

#include "ImageClip.h"
#include "QFile"

cv::Mat ImageClip::toMat() const {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return cv::Mat();

    QByteArray byteArray = file.readAll();
    std::vector data(byteArray.data(), byteArray.data() + byteArray.size());
    cv::Mat image = cv::imdecode(cv::Mat(data), cv::IMREAD_UNCHANGED);

    if (image.empty()) {
        // 如果图像为空，返回空矩阵
        return cv::Mat();
    }

    for (MatEffect *effect: externalEffect) {
        image = effect->apply(image);
    }

    return image;
}
