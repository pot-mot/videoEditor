#ifndef VIDEOCLIP_H
#define VIDEOCLIP_H

#include "clipbase.h"
#include <QRect>
#include <opencv2/core/mat.hpp>

// 视频切片类，继承自 Clip
class VideoClip : public Clip {
public:
    VideoClip(const QString &filePath, int startTime, int offsetTime, int length,
              const QRect &displayArea, qreal opacity, const QString &externalEffect)
        : Clip(ResourceType::Video, filePath, startTime, offsetTime, length),
          displayArea(displayArea), opacity(opacity), externalEffect(externalEffect) {}

    const QRect &getDisplayArea() const { return displayArea; }
    qreal getOpacity() const { return opacity; }
    const QString &getExternalEffect() const { return externalEffect; }

    void setDisplayArea(const QRect &area) { displayArea = area; }
    void setOpacity(qreal value) { opacity = value; }
    void setExternalEffect(const QString &effect) { externalEffect = effect; }

    // 新增方法声明：获取指定时间点的视频帧
    cv::Mat getFrameAtTime(int timeMs) const;

private:
    QRect displayArea;          // 显示区域
    qreal opacity;              // 透明度
    QString externalEffect;     // 外部效果
};

#endif // VIDEOCLIP_H