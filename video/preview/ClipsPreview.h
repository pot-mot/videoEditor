//
// Created by 37702 on 2025/6/2.
//

#ifndef CLIPSPREVIEW_H
#define CLIPSPREVIEW_H
#include <QImage>
#include "../clip/audioclip.h"

class ClipsPreview {
    QImage preview(QList<Clip *> clips, int currentTime, int width, int height);
};



#endif //CLIPSPREVIEW_H
