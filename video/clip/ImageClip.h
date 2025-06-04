#ifndef IMAGECLIP_H
#define IMAGECLIP_H

#include "ClipBase.h"
#include <QList>
#include "../effect/MatEffect.h"

// 图片切片类，继承自 Clip
class ImageClip : public Clip {
public:
    ImageClip(const QString &filePath, int startTime, int offsetTime, int duration,
              const cv::Rect &displayArea, const QList<MatEffect *> &externalEffect)
        : Clip(ResourceType::Image, filePath, startTime, offsetTime, duration),
          displayArea(displayArea), externalEffect(externalEffect) {}

    const cv::Rect &getDisplayArea() const { return displayArea; }
    const QList<MatEffect *> &getExternalEffect() const { return externalEffect; }

    void setDisplayArea(const cv::Rect &area) { displayArea = area; }
    void setExternalEffect(const QList<MatEffect *> &effect) { externalEffect = effect; }

    cv::Mat toMat() const;
private:
    cv::Rect displayArea;          // 显示区域
    QList<MatEffect *> externalEffect;     // 外部效果
};

#endif // IMAGECLIP_H