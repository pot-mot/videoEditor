#include "VideoRender.h"
#include "../clip/VideoClip.h"
#include "../clip/ImageClip.h"
#include "../clip/AudioClip.h"
#include "../clip/TextClip.h"
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>
#include <QFont>
#include <QPainter>
#include <QDebug>
#include <QAudioFormat>

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
        cv::Mat frame = cv::Mat::zeros(height, width, CV_8UC3); // 黑色背景

        for (const auto &clip: clips) {
            if (timeMs >= clip->getStartTime() && timeMs < clip->getStartTime() + clip->getDuration()) {
                switch (clip->getType()) {
                    case ResourceType::Video: {
                        VideoClip *videoClip = static_cast<VideoClip *>(clip);

                        cv::Mat videoFrame = videoClip->getFrameAtTime(timeMs - clip->getStartTime(), fps);
                        if (videoFrame.empty()) {
                            break;
                        }
                        QRect displayArea = videoClip->getDisplayArea();
                        int x = displayArea.x();
                        int y = displayArea.y();
                        int displayWidth = displayArea.width();
                        int displayheight = displayArea.height();

                        // 获取目标区域（在 frame 上的位置和大小）
                        cv::Rect roi(x, y, displayWidth, displayheight);
                        // 计算与 frame 的交集，即“实际可绘制区域”
                        cv::Rect safeRoi = roi & frameRect;

                        // 如果交集为空，说明完全越界，跳过复制
                        if (safeRoi.width > 0 && safeRoi.height > 0) {
                            cv::Mat resizedFrame;
                            cv::resize(videoFrame, resizedFrame, cv::Size(displayWidth, displayheight));

                            // 确保 resizedFrame 是 3 通道 BGR 图像
                            if (resizedFrame.channels() != 3) {
                                cv::cvtColor(resizedFrame, resizedFrame, cv::COLOR_BGRA2BGR); // 假设是带透明通道的 PNG
                            }

                            resizedFrame.copyTo(frame(safeRoi));
                            resizedFrame.release();
                        }

                        videoFrame.release();
                        break;
                    }
                    case ResourceType::Image: {
                        ImageClip *imageClip = static_cast<ImageClip *>(clip);

                        cv::Mat imageFrame = imageClip->toMat();
                        if (imageFrame.empty()) {
                            break;
                        }

                        QRect displayArea = imageClip->getDisplayArea();
                        int x = displayArea.x();
                        int y = displayArea.y();
                        int displayWidth = displayArea.width();
                        int displayheight = displayArea.height();

                        // 获取目标区域（在 frame 上的位置和大小）
                        cv::Rect roi(x, y, displayWidth, displayheight);
                        // 计算与 frame 的交集，即“实际可绘制区域”
                        cv::Rect safeRoi = roi & frameRect;

                        // 如果交集为空，说明完全越界，跳过复制
                        if (safeRoi.width > 0 && safeRoi.height > 0) {
                            cv::Mat resizedFrame;
                            cv::resize(imageFrame, resizedFrame, cv::Size(displayWidth, displayheight));

                            // 确保 resizedFrame 是 3 通道 BGR 图像
                            if (resizedFrame.channels() != 3) {
                                cv::cvtColor(resizedFrame, resizedFrame, cv::COLOR_BGRA2BGR); // 假设是带透明通道的 PNG
                            }

                            resizedFrame.copyTo(frame(safeRoi));
                            resizedFrame.release();
                        }

                        imageFrame.release();
                        break;
                    }
                    case ResourceType::Text: {
                        TextClip *textClip = static_cast<TextClip *>(clip);
                        QString text = textClip->getText();
                        QRect displayArea = textClip->getDisplayArea();
                        int fontFace = cv::FONT_HERSHEY_SIMPLEX;
                        double fontScale = 1.0;
                        int thickness = 2;
                        cv::putText(frame, text.toStdString(),
                                    cv::Point(displayArea.x(), displayArea.y() + displayArea.height()),
                                    fontFace, fontScale, cv::Scalar(255, 255, 255), thickness);
                        break;
                    }
                    case ResourceType::Audio:
                        break;
                    default:
                        qDebug() << "Unsupported clip type.";
                        break;
                }
            }
        }
        qDebug() << "Processing frame at time: " << timeMs;

        writer.write(frame);
    }

    writer.release();
    qDebug() << "Video rendering completed.";
}
