#include "ClipsPreview.h"

#include "../clip/VideoClip.h"
#include "../clip/ImageClip.h"
#include "../clip/TextClip.h"
#include "../../utils/MatImageConvert.h"
#include <QPainter>
#include <opencv2/imgproc.hpp>

QImage ClipsPreview::preview(QList<Clip *> clips, int currentTime, int width, int height, double fps) {
    QImage image(width, height, QImage::Format_RGB888);
    image.fill(Qt::black); // 使用黑色填充背景
    
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    
    foreach (Clip* clip, clips) {
        switch (clip->getType()) {
            case ResourceType::Video: {
                VideoClip *videoClip = static_cast<VideoClip*>(clip);
                QRect displayArea = videoClip->getDisplayArea();
                cv::Mat frame = videoClip->getFrameAtTime(currentTime, fps);
                painter.drawImage(displayArea, MatImageConvert::toImage(frame));
                break;
            }
            case ResourceType::Image: {
                ImageClip *imageClip = static_cast<ImageClip*>(clip);
                QImage img(imageClip->getFilePath());
                if (!img.isNull()) {
                    painter.drawImage(imageClip->getDisplayArea(), img);
                }
                break;
            }
            case ResourceType::Text: {
                TextClip *textClip = static_cast<TextClip*>(clip);
                QRect area = textClip->getDisplayArea();
                painter.setPen(Qt::white);
                painter.setFont(QFont("Arial", area.height()/4)); // 字号自适应区域
                painter.drawText(area, Qt::AlignCenter, textClip->getText());
                break;
            }
            default:
                break;
        }
    }
    
    return image;
}
