//
// Created by 37702 on 2025/6/2.
//

#ifndef CLIPSPREVIEW_H
#define CLIPSPREVIEW_H
#include <QImage>
#include "../clip/AudioClip.h"

class ClipsPreview {
public:
    static QImage preview(QList<Clip *> clips, int currentTime, int width, int height, double fps);
};



#endif //CLIPSPREVIEW_H
