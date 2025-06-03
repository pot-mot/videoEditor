#include <opencv2/opencv.hpp>

#include "ImageClip.h"

cv::Mat ImageClip::toMat() const {
    cv::Mat image = cv::imread(filePath.toStdString(), cv::IMREAD_UNCHANGED);

    if (image.empty()) {
        // 如果图像为空，返回空矩阵
        return cv::Mat();
    }

    return image;
}
