#ifndef IMAGECLIP_H
#define IMAGECLIP_H

#include "clipbase.h"
#include <QRect>

// 图片切片类，继承自 Clip
class ImageClip : public Clip {
public:
    ImageClip(const QString &filePath, int startTime, int offsetTime, int length,
              const QRect &displayArea, qreal opacity, const QString &externalEffect)
        : Clip(ResourceType::Image, filePath, startTime, offsetTime, length),
          displayArea(displayArea), opacity(opacity), externalEffect(externalEffect) {}

    const QRect &getDisplayArea() const { return displayArea; }
    qreal getOpacity() const { return opacity; }
    const QString &getExternalEffect() const { return externalEffect; }

    void setDisplayArea(const QRect &area) { displayArea = area; }
    void setOpacity(qreal value) { opacity = value; }
    void setExternalEffect(const QString &effect) { externalEffect = effect; }

private:
    QRect displayArea;          // 显示区域
    qreal opacity;              // 透明度
    QString externalEffect;     // 外部效果
};

#endif // IMAGECLIP_H