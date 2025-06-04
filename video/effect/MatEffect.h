#ifndef EFFECTBASE_H
#define EFFECTBASE_H

#include <qmap.h>
#include <opencv2/opencv.hpp>

enum class MatEffectType {
    Grayscale,
    Binarize,
    MeanFilter,
    GammaCorrection,
    EdgeDetection
};

// 定义效果基类
class MatEffect {
public:
    MatEffect(MatEffectType type): _type(type) {
    }

    virtual ~MatEffect() {};

    virtual cv::Mat apply(const cv::Mat &image) const = 0;

    MatEffectType type() const { return _type; }

private:
    MatEffectType _type;
};

static QMap<QString, MatEffectType> effectNameMap = {
    {"Grayscale", MatEffectType::Grayscale},
    {"Binarize", MatEffectType::Binarize},
    {"Mean Filter", MatEffectType::MeanFilter},
    {"Gamma Correction", MatEffectType::GammaCorrection},
    {"Edge Detection", MatEffectType::EdgeDetection}
};

static QMap<MatEffectType, QString> effectTypeMap = {
    {MatEffectType::Grayscale, "Grayscale"},
    {MatEffectType::Binarize, "Binarize"},
    {MatEffectType::MeanFilter, "Mean Filter"},
    {MatEffectType::GammaCorrection, "Gamma Correction"},
    {MatEffectType::EdgeDetection, "Edge Detection"}
};


// 灰度效果类
class GrayscaleEffect : public MatEffect {
public:
    GrayscaleEffect(): MatEffect(MatEffectType::Grayscale) {
    }

    cv::Mat apply(const cv::Mat &image) const override;
};

// 二值化效果类
class BinarizeEffect : public MatEffect {
public:
    BinarizeEffect(int threshold = 127) : MatEffect(MatEffectType::Binarize), threshold_(threshold) {
    }

    cv::Mat apply(const cv::Mat &image) const override;

private:
    int threshold_;
};

// 均值滤波效果类
class MeanFilterEffect : public MatEffect {
public:
    MeanFilterEffect(int kernelSize = 3) : MatEffect(MatEffectType::MeanFilter), kernelSize_(kernelSize) {
    }

    cv::Mat apply(const cv::Mat &image) const override;

private:
    int kernelSize_;
};

// 伽马校正效果类
class GammaCorrectionEffect : public MatEffect {
public:
    GammaCorrectionEffect(double gamma = 1.0) : MatEffect(MatEffectType::GammaCorrection), gamma_(gamma) {
    }

    cv::Mat apply(const cv::Mat &image) const override;

private:
    double gamma_;
};

// 边缘检测效果类
class EdgeDetectionEffect : public MatEffect {
public:
    EdgeDetectionEffect() : MatEffect(MatEffectType::EdgeDetection) {
    }

    cv::Mat apply(const cv::Mat &image) const override;
};

class EffectFactory {
public:
    static MatEffect *createEffect(MatEffectType type) {
        switch (type) {
            case MatEffectType::Grayscale: return new GrayscaleEffect();
            case MatEffectType::Binarize: return new BinarizeEffect();
            case MatEffectType::MeanFilter: return new MeanFilterEffect();
            case MatEffectType::GammaCorrection: return new GammaCorrectionEffect();
            case MatEffectType::EdgeDetection: return new EdgeDetectionEffect();
            default: return nullptr;
        }
    }
};

#endif // EFFECTBASE_H
