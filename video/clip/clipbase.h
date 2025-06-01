#ifndef CLIPBASE_H
#define CLIPBASE_H

#include <QString>

// 新增枚举类型，用于区分资源类型
enum class ResourceType {
    Video,
    Audio,
    Image
};

// 基类 Clip，包含通用属性
class Clip {
public:
    Clip(ResourceType type, const QString &filePath, int startTime, int offsetTime, int length)
        : type(type), filePath(filePath), startTime(startTime), offsetTime(offsetTime), length(length) {}

    virtual ~Clip() = default;

    ResourceType getType() const { return type; }
    const QString &getFilePath() const { return filePath; }
    int getStartTime() const { return startTime; }
    int getOffsetTime() const { return offsetTime; }
    int getLength() const { return length; }

protected:
    ResourceType type;          // 资源类型
    QString filePath;           // 原始资源文件路径
    int startTime;              // 切片在主时间轴上的起始时间（单位：毫秒）
    int offsetTime;             // 切片相对资源文件开始时的偏移量（单位：毫秒）
    int length;                 // 切片时长（单位：毫秒）
};

#endif // CLIPBASE_H