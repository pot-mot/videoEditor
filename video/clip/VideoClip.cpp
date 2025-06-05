#include "VideoClip.h"
#include <opencv2/opencv.hpp>
#include <QDebug>

cv::Mat VideoClip::getFrameAtTime(int timeMs) const {
    if (timeMs < 0 || timeMs >= duration) {
        return cv::Mat(); // 返回空矩阵表示无效时间
    }

    cv::VideoCapture cap(filePath.toStdString());
    if (!cap.isOpened()) {
        return cv::Mat();
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    int frameNumber = static_cast<int>(timeMs * fps / 1000.0);

    cap.set(cv::CAP_PROP_POS_FRAMES, frameNumber);
    
    cv::Mat frame;
    if (cap.read(frame)) {
        for (MatEffect *effect : externalEffect) {
            frame = effect->apply(frame);
        }

        return frame;
    }
    return cv::Mat();
}