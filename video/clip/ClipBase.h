#ifndef CLIPBASE_H
#define CLIPBASE_H

#include <QString>

// 新增枚举类型，用于区分资源类型
enum class ResourceType {
    Video,
    Audio,
    Image,
    Text,
};

// 基类 Clip，包含所有切片的公共属性和方法
class Clip {
public:
    Clip(ResourceType type, const QString &filePath, int startTime, int offsetTime, int duration)
        : type(type), filePath(filePath), startTime(startTime), offsetTime(offsetTime), duration(duration) {}

    ResourceType getType() const { return type; }
    const QString &getFilePath() const { return filePath; }
    int getStartTime() const { return startTime; }
    int getOffsetTime() const { return offsetTime; }
    int getDuration() const { return duration; }

    void setFilePath(const QString &path) { filePath = path; }
    void setStartTime(int time) { startTime = time; }
    void setOffsetTime(int time) { offsetTime = time; }
    void setDuration(int during) { duration = during; }

private:
    ResourceType type;          // 资源类型
    QString filePath;           // 文件路径
    int startTime;              // 开始时间
    int offsetTime;             // 相对文件开始的偏移时间，如果是文本和图片，则为0
    int duration;                 // 持续时间
};

#endif // CLIPBASE_H