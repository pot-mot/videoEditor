#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include "clipbase.h"

// 音频切片类，继承自 Clip
class AudioClip : public Clip {
public:
    AudioClip(const QString &filePath, int startTime, int offsetTime, int length,
              qreal volume, const QString &externalEffect)
        : Clip(ResourceType::Audio, filePath, startTime, offsetTime, length),
          volume(volume), externalEffect(externalEffect) {}

    qreal getVolume() const { return volume; }
    const QString &getExternalEffect() const { return externalEffect; }

private:
    qreal volume;               // 响度
    QString externalEffect;     // 外部效果
};

#endif // AUDIOCLIP_H