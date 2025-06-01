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
    Clip(ResourceType type, const QString &filePath, int startTime, int offsetTime, int length)
        : type(type), filePath(filePath), startTime(startTime), offsetTime(offsetTime), length(length) {}

    ResourceType getType() const { return type; }
    const QString &getFilePath() const { return filePath; }
    int getStartTime() const { return startTime; }
    int getOffsetTime() const { return offsetTime; }
    int getLength() const { return length; }

    void setFilePath(const QString &path) { filePath = path; }
    void setStartTime(int time) { startTime = time; }
    void setOffsetTime(int time) { offsetTime = time; }
    void setLength(int len) { length = len; }

private:
    ResourceType type;          // 资源类型
    QString filePath;           // 文件路径
    int startTime;              // 开始时间
    int offsetTime;             // 偏移时间
    int length;                 // 持续时间
};

#endif // CLIPBASE_H