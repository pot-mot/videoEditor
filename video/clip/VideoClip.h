#ifndef VIDEOCLIP_H
#define VIDEOCLIP_H

#include "ClipBase.h"
#include <QRect>
#include <opencv2/core/mat.hpp>

// 视频切片类，继承自 Clip
class VideoClip : public Clip {
public:
    VideoClip(const QString &filePath, int startTime, int offsetTime, int duration,
              const QRect &displayArea, const QString &externalEffect)
        : Clip(ResourceType::Video, filePath, startTime, offsetTime, duration),
          displayArea(displayArea), externalEffect(externalEffect) {}

    const QRect &getDisplayArea() const { return displayArea; }
    const QString &getExternalEffect() const { return externalEffect; }

    void setDisplayArea(const QRect &area) { displayArea = area; }
    void setExternalEffect(const QString &effect) { externalEffect = effect; }

    // 新增方法声明：获取指定时间点的视频帧
    cv::Mat getFrameAtTime(int timeMs, double fps) const;

private:
    QRect displayArea;          // 显示区域
    QString externalEffect;     // 外部效果
};

#endif // VIDEOCLIP_H