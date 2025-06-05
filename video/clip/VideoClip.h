#ifndef VIDEOCLIP_H
#define VIDEOCLIP_H

#include "ClipBase.h"
#include <QList>
#include <opencv2/core/mat.hpp>
#include "../effect/MatEffect.h"

// 视频切片类，继承自 Clip
class VideoClip : public Clip {
public:
    VideoClip(const QString &filePath, int startTime, int offsetTime, int duration,
              int maxDuration, const cv::Rect &displayArea, const QList<MatEffect *> &externalEffect)
        : Clip(ResourceType::Video, filePath, startTime, offsetTime, duration),
          maxDuration(maxDuration), displayArea(displayArea), externalEffect(externalEffect) {}

    const int getMaxDuration() const { return maxDuration; }
    const cv::Rect &getDisplayArea() const { return displayArea; }
    const QList<MatEffect *> &getExternalEffect() const { return externalEffect; }

    void setDisplayArea(const cv::Rect &area) { displayArea = area; }
    void setExternalEffect(const QList<MatEffect *> &effect) { externalEffect = effect; }

    void setOffsetTime(int time) override {
        if (time <= maxDuration) {
            offsetTime = time;
        } else {
            offsetTime = maxDuration;
        }
    }
    void setDuration(int during) override {
        if (during <= maxDuration) {
            duration = during;
        } else {
            duration = maxDuration;
        }
    }

    // 新增方法声明：获取指定时间点的视频帧
    cv::Mat getFrameAtTime(int timeMs) const;

private:
    int maxDuration;            // 最长时长
    cv::Rect displayArea;          // 显示区域
    QList<MatEffect *> externalEffect;     // 外部效果
};

#endif // VIDEOCLIP_H