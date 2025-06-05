//
// Created by 37702 on 2025/6/2.
//

#ifndef CLIPSPREVIEW_H
#define CLIPSPREVIEW_H
#include <QImage>
#include "../clip/AudioClip.h"
#include <opencv2/imgproc.hpp>

class ClipsPreview {
public:
    /**
     * 将图片缩放为指定尺寸后绘制到帧中
     * @param frame 目标帧
     * @param image 图片
     * @param displayArea 绘制区域
     * @param frameRect 帧尺寸，用于计算交集判断范围
     */
    static void drawResizedImageToFrame(
        cv::Mat &frame,
        const cv::Mat &image,
        const cv::Rect &displayArea,
        const cv::Rect &frameRect
    );

    static cv::Mat preview(
        QList<Clip *> clips,
        int currentTime,
        const cv::Rect &frameRect
    );
};



#endif //CLIPSPREVIEW_H
