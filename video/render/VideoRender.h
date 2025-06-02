//
// Created by 37702 on 2025/6/2.
//

#ifndef VIDEORENDER_H
#define VIDEORENDER_H
#include <opencv2/core/cvstd.hpp>

#include "../clip/clipbase.h"

class VideoRender {
public:
    static void render(const cv::String &outputPath, QList<Clip *> clips);
};



#endif //VIDEORENDER_H
