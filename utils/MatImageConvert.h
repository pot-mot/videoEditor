//
// Created by 37702 on 2025/6/2.
//

#ifndef MATTOIMAGE_H
#define MATTOIMAGE_H
#include <QImage>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>


class MatImageConvert {
public:
    static QImage toImage(cv::Mat mat) {
        if (!mat.empty()) {
            // 转换为RGB格式
            cv::Mat rgbFrame;
            cv::cvtColor(mat, rgbFrame, cv::COLOR_BGR2RGB);

            // 转换为 QImage
            return QImage(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
        } else {
            return QImage();
        }
    }
};



#endif //MATTOIMAGE_H
