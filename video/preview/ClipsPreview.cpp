#include "ClipsPreview.h"

#include "../clip/VideoClip.h"
#include "../clip/ImageClip.h"
#include "../clip/TextClip.h"
#include "../../utils/MatImageConvert.h"
#include <QPainter>
#include <opencv2/imgproc.hpp>

QImage ClipsPreview::preview(QList<Clip *> clips, int currentTime, int width, int height, double fps) {
    cv::Rect frameRect(0, 0, width, height);
    cv::Mat frame = cv::Mat::zeros(height, width, CV_8UC3);

    for (const auto &clip: clips) {
        if (currentTime >= clip->getStartTime() && currentTime < clip->getStartTime() + clip->getDuration()) {
            switch (clip->getType()) {
                case ResourceType::Video: {
                    VideoClip *videoClip = static_cast<VideoClip *>(clip);

                    cv::Mat videoFrame = videoClip->getFrameAtTime(currentTime - clip->getStartTime(), fps);
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
                default:
                    break;
            }
        }
    }

    return MatImageConvert::toImage(frame);
}
