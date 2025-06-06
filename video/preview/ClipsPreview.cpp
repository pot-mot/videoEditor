#include "ClipsPreview.h"

#include "../clip/VideoClip.h"
#include "../clip/ImageClip.h"
#include "../clip/TextClip.h"
#include <QPainter>
#include <opencv2/imgproc.hpp>

cv::Mat ClipsPreview::preview(QList<Clip *> clips, int currentTime, const cv::Rect &frameRect) {
    cv::Mat frame = cv::Mat::zeros(frameRect.height, frameRect.width, CV_8UC4);

    for (const auto &clip: clips) {
        if (currentTime >= clip->getStartTime() && currentTime < clip->getStartTime() + clip->getDuration()) {
            switch (clip->getType()) {
                case ResourceType::Video: {
                    VideoClip *videoClip = static_cast<VideoClip *>(clip);

                    cv::Mat videoFrame = videoClip->getFrameAtTime(currentTime - clip->getStartTime());
                    if (videoFrame.empty()) {
                        break;
                    }
                    drawResizedImageToFrame(frame, videoFrame, videoClip->getDisplayArea(), frameRect);
                    break;
                }
                case ResourceType::Image: {
                    ImageClip *imageClip = static_cast<ImageClip *>(clip);

                    cv::Mat imageFrame = imageClip->toMat();
                    if (imageFrame.empty()) {
                        break;
                    }
                    drawResizedImageToFrame(frame, imageFrame, imageClip->getDisplayArea(), frameRect);
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

    return frame;
}

void ClipsPreview::drawResizedImageToFrame(
    cv::Mat &frame,
    const cv::Mat &image,
    const cv::Rect &displayArea,
    const cv::Rect &frameRect
) {
    // 计算与 frame 的交集，即“实际可绘制区域”
    cv::Rect safeArea = displayArea & frameRect;

    // 如果交集为空，说明完全越界，跳过复制
    if (safeArea.width <= 0 || safeArea.height <= 0) {
        return;
    }

    // 缩放图像到目标尺寸
    cv::Mat resizedFrame;
    cv::resize(image, resizedFrame, cv::Size(displayArea.width, displayArea.height));

    // 计算 resizedFrame 上的有效区域（对应于 safeRoi 在 roi 中的位置）
    int srcX = safeArea.x - displayArea.x;
    int srcY = safeArea.y - displayArea.y;
    cv::Rect srcRect(srcX, srcY, safeArea.width, safeArea.height);

    // 检查 srcRect 是否在 resizedFrame 的范围内
    if (srcRect.x >= 0 && srcRect.y >= 0 &&
        srcRect.width > 0 && srcRect.height > 0 &&
        (srcRect.x + srcRect.width) <= resizedFrame.cols &&
        (srcRect.y + srcRect.height) <= resizedFrame.rows) {
        // 提取有效区域
        cv::Mat validRegion = resizedFrame(srcRect);

        switch (validRegion.type()) {
            case CV_8UC4: {
                validRegion.copyTo(frame(safeArea));
            }

            case CV_8UC1: {
                // 灰度图 → BGRA
                cv::Mat bgr, bgra;
                cv::cvtColor(validRegion, bgr, cv::COLOR_GRAY2BGR);
                cv::cvtColor(bgr, bgra, cv::COLOR_BGR2BGRA);
                bgra.copyTo(frame(safeArea));
                break;
            }

            case CV_8UC3: {
                // BGR → BGRA（不透明）
                cv::Mat bgra;
                cv::cvtColor(validRegion, bgra, cv::COLOR_BGR2BGRA);
                bgra.copyTo(frame(safeArea));
                break;
            }

            default:
                break;
        }

        validRegion.release();
    }

    // 释放资源
    resizedFrame.release();
}
