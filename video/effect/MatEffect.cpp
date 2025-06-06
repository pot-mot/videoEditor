#include "MatEffect.h"

cv::Mat GrayscaleEffect::apply(const cv::Mat &image) const {
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    return gray;
}

cv::Mat BinarizeEffect::apply(const cv::Mat &image) const {
    cv::Mat gray, binary;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, binary, threshold_, 255, cv::THRESH_BINARY);
    return binary;
}

cv::Mat MeanFilterEffect::apply(const cv::Mat &image) const {
    cv::Mat result;
    cv::blur(image, result, cv::Size(kernelSize_, kernelSize_));
    return result;
}

cv::Mat GammaCorrectionEffect::apply(const cv::Mat &image) const {
    cv::Mat result;
    image.convertTo(result, -1, 1.0 / gamma_);
    return result;
}

cv::Mat EdgeDetectionEffect::apply(const cv::Mat &image) const {
    cv::Mat gray, edges;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, 100, 200);
    return edges;
}

cv::Mat FeatureDetectionEffect::apply(const cv::Mat &image) const {
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keypoints;
    orb->detect(grayImage, keypoints);

    cv::Mat outputImage;
    cv::drawKeypoints(image, keypoints, outputImage, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);

    return outputImage;
}

cv::Mat OpacityEffect::apply(const cv::Mat &image) const {
    // 1. 转换为BGRA格式（确保Alpha通道存在）
    cv::Mat inputBGRA;
    if (image.type() != CV_8UC4) {
        if (image.channels() == 1) {
            cv::Mat temp;
            cv::cvtColor(image, temp, cv::COLOR_GRAY2BGR);
            cv::cvtColor(temp, inputBGRA, cv::COLOR_BGR2BGRA);
        } else if (image.channels() == 3) {
            cv::cvtColor(image, inputBGRA, cv::COLOR_BGR2BGRA);
        } else {
            inputBGRA = image.clone();
        }
    } else {
        inputBGRA = image.clone();
    }

    // 2. 调整Alpha通道
    for (int y = 0; y < inputBGRA.rows; ++y) {
        for (int x = 0; x < inputBGRA.cols; ++x) {
            cv::Vec4b &pixel = inputBGRA.at<cv::Vec4b>(y, x);
            pixel[3] = static_cast<uchar>(pixel[3] * opacity_);
        }
    }

    return inputBGRA;
}

cv::Mat RotateEffect::apply(const cv::Mat &image) const {
    // 1. 转换为BGRA格式（确保Alpha通道存在）
    cv::Mat inputBGRA;
    if (image.type() != CV_8UC4) {
        if (image.channels() == 1) {
            cv::Mat temp;
            cv::cvtColor(image, temp, cv::COLOR_GRAY2BGR);
            cv::cvtColor(temp, inputBGRA, cv::COLOR_BGR2BGRA);
        } else if (image.channels() == 3) {
            cv::cvtColor(image, inputBGRA, cv::COLOR_BGR2BGRA);
        } else {
            inputBGRA = image.clone();
        }
    } else {
        inputBGRA = image.clone();
    }

    // 2. 计算旋转矩阵
    cv::Point2f center(inputBGRA.cols / 2.0f, inputBGRA.rows / 2.0f);
    cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, degree_, 1.0);

    // 3. 计算旋转后的新尺寸
    std::vector<cv::Point2f> corners = {
        {0, 0},
        {static_cast<float>(inputBGRA.cols), 0},
        {0, static_cast<float>(inputBGRA.rows)},
        {static_cast<float>(inputBGRA.cols), static_cast<float>(inputBGRA.rows)}
    };
    std::vector<cv::Point2f> rotatedCorners;
    cv::transform(corners, rotatedCorners, rotationMatrix);

    float xMin = std::numeric_limits<float>::max();
    float xMax = -std::numeric_limits<float>::min();
    float yMin = std::numeric_limits<float>::max();
    float yMax = -std::numeric_limits<float>::min();

    for (const auto &pt : rotatedCorners) {
        xMin = std::min(xMin, pt.x);
        xMax = std::max(xMax, pt.x);
        yMin = std::min(yMin, pt.y);
        yMax = std::max(yMax, pt.y);
    }

    // 4. 调整旋转矩阵以包含完整图像
    cv::Mat adjustedMatrix = rotationMatrix.clone();
    adjustedMatrix.at<double>(0, 2) += (0 - xMin);
    adjustedMatrix.at<double>(1, 2) += (0 - yMin);

    int newWidth = static_cast<int>(xMax - xMin);
    int newHeight = static_cast<int>(yMax - yMin);

    // 5. 执行仿射变换（双线性插值 + 透明填充）
    cv::Mat rotated;
    cv::warpAffine(
        inputBGRA, rotated, adjustedMatrix,
        cv::Size(newWidth, newHeight),
        cv::INTER_LINEAR,
        cv::BORDER_CONSTANT,
        cv::Scalar(0, 0, 0, 0)  // 透明背景
    );

    return rotated;
}