#include "VideoRender.h"
#include <opencv2/opencv.hpp>
#include <QPainter>
#include <QDebug>
#include "../preview/ClipsPreview.h"

void VideoRender::render(const cv::String &outputPath, QList<Clip *> clips, int width, int height, double fps) {
    if (clips.isEmpty()) {
        qDebug() << "No clips to render.";
        return;
    }

    // 计算总时长
    int totalDuration = 0;
    for (const auto &clip: clips) {
        int end = clip->getStartTime() + clip->getDuration();
        if (end > totalDuration) {
            totalDuration = end;
        }
    }

    int frameStep = 1000 / fps;

    // 初始化视频写入器
    cv::VideoWriter writer(outputPath, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, cv::Size(width, height));
    if (!writer.isOpened()) {
        qDebug() << "Failed to open output video file.";
        return;
    }

    cv::Rect frameRect(0, 0, width, height);

    // 逐帧处理
    for (int timeMs = 0; timeMs < totalDuration; timeMs += frameStep) {
        cv::Mat frame = ClipsPreview::preview(clips, timeMs, fps, frameRect);

        writer.write(frame);

        qDebug() << "Processing frame at time: " << timeMs;
    }

    writer.release();
    qDebug() << "Video rendering completed.";
}
