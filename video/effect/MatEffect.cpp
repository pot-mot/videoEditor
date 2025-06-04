#include "MatEffect.h"

cv::Mat GrayscaleEffect::apply(const cv::Mat &image) const {
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    return gray;
}

cv::Mat BinarizeEffect::apply(const cv::Mat &image) const {
    cv::Mat gray, binary;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, binary, threshold_, 255, cv::THRESH_BINARY);
    return binary;
}

cv::Mat MeanFilterEffect::apply(const cv::Mat &image) const {
    cv::Mat result;
    cv::blur(image, result, cv::Size(kernelSize_, kernelSize_));
    return result;
}

cv::Mat GammaCorrectionEffect::apply(const cv::Mat &image) const {
    cv::Mat result;
    image.convertTo(result, -1, 1.0 / gamma_);
    return result;
}

cv::Mat EdgeDetectionEffect::apply(const cv::Mat &image) const {
    cv::Mat gray, edges;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, 100, 200);
    return edges;
}
