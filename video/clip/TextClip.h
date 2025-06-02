#ifndef TEXTCLIP_H
#define TEXTCLIP_H

#include "ClipBase.h"
#include <QRect>

// 文本切片类，继承自 Clip
class TextClip : public Clip {
public:
    TextClip(const QString &text, int startTime, int offsetTime, int length,
             const QRect &displayArea, qreal opacity, const QString &externalEffect)
        : Clip(ResourceType::Text, "", startTime, offsetTime, length),
          text(text), displayArea(displayArea), opacity(opacity), externalEffect(externalEffect) {}

    const QString &getText() const { return text; }
    const QRect &getDisplayArea() const { return displayArea; }
    qreal getOpacity() const { return opacity; }
    const QString &getExternalEffect() const { return externalEffect; }

    void setText(const QString &newText) { text = newText; }
    void setDisplayArea(const QRect &area) { displayArea = area; }
    void setOpacity(qreal value) { opacity = value; }
    void setExternalEffect(const QString &effect) { externalEffect = effect; }

private:
    QString text;               // 文本内容
    QRect displayArea;          // 显示区域
    qreal opacity;              // 透明度
    QString externalEffect;     // 外部效果
};

#endif // TEXTCLIP_H