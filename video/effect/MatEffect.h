#ifndef EFFECTBASE_H
#define EFFECTBASE_H

#include <opencv2/opencv.hpp>

// 定义效果基类
class MatEffect {
public:
    virtual ~MatEffect() {}
    virtual cv::Mat apply(const cv::Mat& image) const = 0;
};

// 灰度效果类
class GrayscaleEffect : public MatEffect {
public:
    cv::Mat apply(const cv::Mat& image) const override;
};

// 二值化效果类
class BinarizeEffect : public MatEffect {
public:
    BinarizeEffect(int threshold) : threshold_(threshold) {}
    cv::Mat apply(const cv::Mat& image) const override;
private:
    int threshold_;
};

// 均值滤波效果类
class MeanFilterEffect : public MatEffect {
public:
    cv::Mat apply(const cv::Mat& image) const override;
};

// 伽马校正效果类
class GammaCorrectionEffect : public MatEffect {
public:
    GammaCorrectionEffect(double gamma) : gamma_(gamma) {}
    cv::Mat apply(const cv::Mat& image) const override;
private:
    double gamma_;
};

// 边缘检测效果类
class EdgeDetectionEffect : public MatEffect {
public:
    cv::Mat apply(const cv::Mat& image) const override;
};

#endif // EFFECTBASE_H