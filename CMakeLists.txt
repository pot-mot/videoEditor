cmake_minimum_required(VERSION 3.19)
project(videoEditor LANGUAGES CXX)

set(OpenCV_DIR E:/opencv/OpenCV-MinGW-Build-OpenCV-4.1.1-x64/include/opencv2)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Multimedia MultimediaWidgets)
find_package(OpenCV REQUIRED)

qt_standard_project_setup()

qt_add_executable(videoEditor
        WIN32 MACOSX_BUNDLE
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

        image/imageprocessor.h
        image/imageprocessor.cpp
        history/commandhistory.h
        history/commandhistory.cpp
        image/imageeditor.h
        image/imageeditor.cpp
        image/imageeditor.ui
        video/videoeditor.h
        video/videoeditor.cpp
        video/videoeditor.ui
        video/videoprocessor.h
        video/videoprocessor.cpp
        video/timeline/videotimeline.cpp
        video/timeline/videotimeline.h
        video/timeline/videotimeline.ui
        file/fileviewer.cpp
        file/fileviewer.h
        file/fileviewer.ui

        video/clip/clipbase.h
        video/clip/imageclip.h
        video/clip/audioclip.h
        video/clip/videoclip.h
        video/clip/textclip.h
        video/render/VideoRender.cpp
        video/render/VideoRender.h
        video/preview/ClipsPreview.cpp
        video/preview/ClipsPreview.h
        video/clip/videoclip.cpp
        utils/MatImageConvert.h
        video/clip/ImageClip.cpp
        video/timeline/videotimelinetrack.cpp
        video/timeline/videotimelinetrack.h
        video/timeline/videotimelinetrack.ui
        video/effect/MatEffect.h
        video/effect/MatEffect.cpp
        video/clip/form/ClipForm.cpp
        video/clip/form/ClipForm.h
        video/clip/form/ClipForm.ui
)

target_include_directories(videoEditor
        PRIVATE
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(videoEditor
        PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Multimedia
        Qt::MultimediaWidgets
        ${OpenCV_LIBS}
)

include(GNUInstallDirs)

install(TARGETS videoEditor
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
        TARGET videoEditor
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
