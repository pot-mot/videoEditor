#include "VideoRender.h"
#include "../clip/videoclip.h"
#include "../clip/imageclip.h"
#include "../clip/audioclip.h"
#include "../clip/textclip.h"
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>
#include <QFont>
#include <QPainter>
#include <QDebug>

void VideoRender::render(const cv::String &outputPath, QList<Clip *> clips) {
    if (clips.isEmpty()) {
        qDebug() << "No clips to render.";
        return;
    }

    // 获取第一个视频切片的分辨率作为输出视频的分辨率
    int width = 1280; // 默认宽度
    int height = 720; // 默认高度
    for (const auto &clip: clips) {
        if (clip->getType() == ResourceType::Video) {
            VideoClip *videoClip = static_cast<VideoClip *>(clip);
            cv::VideoCapture cap(videoClip->getFilePath().toStdString());
            if (cap.isOpened()) {
                width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
                height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
                break;
            }
        }
    }

    cv::VideoWriter writer(outputPath, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 60, cv::Size(width, height));

    if (!writer.isOpened()) {
        qDebug() << "Failed to open output video file.";
        return;
    }

    // 处理每个切片
    for (const auto &clip: clips) {
        switch (clip->getType()) {
            case ResourceType::Video: {
                VideoClip *videoClip = static_cast<VideoClip *>(clip);
                cv::VideoCapture cap(videoClip->getFilePath().toStdString());
                if (!cap.isOpened()) {
                    qDebug() << "Failed to open video file:" << videoClip->getFilePath();
                    continue;
                }

                cv::Mat frame;
                while (cap.read(frame)) {
                    // 处理视频帧
                    cv::Mat overlay = frame.clone();
                    QRect qrect = videoClip->getDisplayArea();
                    cv::rectangle(overlay,
                                  cv::Rect(qrect.x(), qrect.y(), qrect.width(), qrect.height()),
                                  cv::Scalar(0, 255, 0),
                                  1);
                    cv::addWeighted(overlay, 0.5, frame, 0.5, 0, frame);
                    writer.write(frame); // 直接使用BGR格式Mat写入，避免QImage转换
                }
                break;
            }
            case ResourceType::Image: {
                ImageClip *imageClip = static_cast<ImageClip *>(clip);
                QImage image(imageClip->getFilePath());
                if (image.isNull()) {
                    qDebug() << "Failed to load image file:" << imageClip->getFilePath();
                    continue;
                }

                // 创建空白帧
                cv::Mat frame(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
                cv::Mat overlay = frame.clone();

                // 将图片绘制到帧上 - 增加颜色空间转换
                cv::Rect roi(imageClip->getDisplayArea().x(), imageClip->getDisplayArea().y(),
                             imageClip->getDisplayArea().width(), imageClip->getDisplayArea().height());
                cv::Mat imageMat = cv::Mat(image.height(), image.width(), CV_8UC3, image.bits(), image.bytesPerLine());
                cv::Mat bgrImage;
                cv::cvtColor(imageMat, bgrImage, cv::COLOR_RGB2BGR); // 修正颜色空间
                bgrImage.copyTo(overlay(roi));

                // 移除QImage转换，直接使用Mat写入
                writer.write(overlay); // 保持BGR格式直接输出
                break;
            }
            case ResourceType::Text: {
                TextClip *textClip = static_cast<TextClip *>(clip);

                // 创建空白帧
                cv::Mat frame(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
                cv::Mat overlay = frame.clone();

                cv::putText(overlay, textClip->getText().toStdString(),
                            cv::Point(textClip->getDisplayArea().x(), textClip->getDisplayArea().y()),
                            cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
                writer.write(overlay); // 保持BGR格式直接输出
                break;
            }
            case ResourceType::Audio: {
                // 音频处理（这里可以添加音频混合逻辑）
                break;
            }
            default:
                qDebug() << "Unsupported clip type.";
                break;
        }
    }

    writer.release();
    qDebug() << "Video rendering completed.";
}
