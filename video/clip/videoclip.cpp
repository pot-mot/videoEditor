#include "videoclip.h"
#include <opencv2/opencv.hpp>

cv::Mat VideoClip::getFrameAtTime(int timeMs) const {
    // 计算相对于剪辑开始的时间
    int relativeTime = timeMs - getStartTime();
    if (relativeTime < 0 || relativeTime >= getLength()) {
        return cv::Mat(); // 返回空矩阵表示无效时间
    }

    cv::VideoCapture cap(getFilePath().toStdString());
    if (!cap.isOpened()) {
        return cv::Mat();
    }

    // 计算目标帧号（假设帧率30fps）
    double fps = cap.get(cv::CAP_PROP_FPS);
    int frameNumber = static_cast<int>(fps * relativeTime / 1000.0);
    
    // 定位到目标帧
    cap.set(cv::CAP_PROP_POS_FRAMES, frameNumber);
    
    cv::Mat frame;
    if (cap.read(frame)) {
        return frame;
    }
    return cv::Mat();
}