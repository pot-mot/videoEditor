#include "VideoClip.h"
#include <opencv2/opencv.hpp>

cv::Mat VideoClip::getFrameAtTime(int timeMs, double fps) const {
    if (timeMs < 0 || timeMs >= duration) {
        return cv::Mat(); // 返回空矩阵表示无效时间
    }

    cv::VideoCapture cap(filePath.toStdString());
    if (!cap.isOpened()) {
        return cv::Mat();
    }

    int frameNumber = static_cast<int>(fps * timeMs / 1000.0);
    
    cap.set(cv::CAP_PROP_POS_FRAMES, frameNumber);
    
    cv::Mat frame;
    if (cap.read(frame)) {
        return frame;
    }
    return cv::Mat();
}