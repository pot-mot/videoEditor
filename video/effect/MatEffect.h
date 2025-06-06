#ifndef EFFECTBASE_H
#define EFFECTBASE_H

#include <QLabel>
#include <qmap.h>
#include <QSlider>
#include <qspinbox.h>
#include <QVBoxLayout>
#include <QWidget>
#include <opencv2/opencv.hpp>

enum class MatEffectType {
    Grayscale,
    Binarize,
    MeanFilter,
    GammaCorrection,
    EdgeDetection,
    FeatureDetection,
    Opacity,
    Rotate
};

// 定义效果基类
class MatEffect {
public:
    MatEffect(MatEffectType type): _type(type) {
    }

    virtual ~MatEffect() {
    };

    virtual cv::Mat apply(const cv::Mat &image) const = 0;

    virtual bool isExclusive() const { return false; }

    MatEffectType type() const { return _type; }

private:
    MatEffectType _type;
};

static QMap<QString, MatEffectType> effectNameMap = {
    {"Grayscale", MatEffectType::Grayscale},
    {"Binarize", MatEffectType::Binarize},
    {"Mean Filter", MatEffectType::MeanFilter},
    {"Opacity", MatEffectType::Opacity},
    {"Rotate", MatEffectType::Rotate},
    {"Gamma Correction", MatEffectType::GammaCorrection},
    {"Edge Detection", MatEffectType::EdgeDetection},
    {"Feature Detection", MatEffectType::FeatureDetection}
};

static QMap<MatEffectType, QString> effectTypeMap = {
    {MatEffectType::Grayscale, "Grayscale"},
    {MatEffectType::Binarize, "Binarize"},
    {MatEffectType::MeanFilter, "Mean Filter"},
    {MatEffectType::Opacity, "Opacity"},
    {MatEffectType::Rotate, "Rotate"},
    {MatEffectType::GammaCorrection, "Gamma Correction"},
    {MatEffectType::EdgeDetection, "Edge Detection"},
    {MatEffectType::FeatureDetection, "Feature Detection"}
};


// 灰度效果类
class GrayscaleEffect : public MatEffect {
public:
    GrayscaleEffect(): MatEffect(MatEffectType::Grayscale) {
    }

    cv::Mat apply(const cv::Mat &image) const override;

    bool isExclusive() const override { return false; }
};

// 二值化效果类
class BinarizeEffect : public MatEffect {
public:
    BinarizeEffect(int threshold = 127) : MatEffect(MatEffectType::Binarize), threshold_(threshold) {
    }

    cv::Mat apply(const cv::Mat &image) const override;

    bool isExclusive() const override { return true; }

private:
    int threshold_;
};

// 均值滤波效果类
class MeanFilterEffect : public MatEffect {
public:
    MeanFilterEffect(int kernelSize = 3) : MatEffect(MatEffectType::MeanFilter), kernelSize_(kernelSize) {
    }

    cv::Mat apply(const cv::Mat &image) const override;

    bool isExclusive() const override { return false; }

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

    bool isExclusive() const override { return true; }
};

// 特征识别效果类
class FeatureDetectionEffect : public MatEffect {
public:
    FeatureDetectionEffect(): MatEffect(MatEffectType::FeatureDetection) {
    }

    cv::Mat apply(const cv::Mat &image) const override;

    bool isExclusive() const override { return true; }
};

class OpacityEffect : public MatEffect {
public:
    OpacityEffect(double opacity = 0.5) : MatEffect(MatEffectType::Opacity), opacity_(opacity) {
    }

    cv::Mat apply(const cv::Mat &image) const override;

private:
    double opacity_;
};

class RotateEffect : public MatEffect {
public:
    RotateEffect(int degree = 45) : MatEffect(MatEffectType::Rotate), degree_(degree) {

    }

    cv::Mat apply(const cv::Mat &image) const override;

private:
    int degree_;
};

class EffectFactory {
public:
    static MatEffect *createEffect(MatEffectType type) {
        switch (type) {
            case MatEffectType::Grayscale: return new GrayscaleEffect();
            case MatEffectType::Binarize: return new BinarizeEffect();
            case MatEffectType::MeanFilter: return new MeanFilterEffect();
            case MatEffectType::Opacity: return new OpacityEffect();
            case MatEffectType::Rotate: return new RotateEffect();
            case MatEffectType::GammaCorrection: return new GammaCorrectionEffect();
            case MatEffectType::EdgeDetection: return new EdgeDetectionEffect();
            case MatEffectType::FeatureDetection: return new FeatureDetectionEffect();
            default: return nullptr;
        }
    }
};

#endif // EFFECTBASE_H
