#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include "ClipBase.h"

// 音频切片类，继承自 Clip
class AudioClip : public Clip {
public:
    AudioClip(const QString &filePath, int startTime, int offsetTime, int duration,
              qreal volume, const QString &externalEffect)
        : Clip(ResourceType::Audio, filePath, startTime, offsetTime, duration),
          volume(volume), externalEffect(externalEffect) {}

    qreal getVolume() const { return volume; }
    const QString &getExternalEffect() const { return externalEffect; }

    void setVolume(qreal value) { volume = value; }
    void setExternalEffect(const QString &effect) { externalEffect = effect; }

private:
    qreal volume;               // 响度
    QString externalEffect;     // 外部效果
};

#endif // AUDIOCLIP_H