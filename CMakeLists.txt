cmake_minimum_required(VERSION 3.19)

#set(CMAKE_TOOLCHAIN_FILE "D:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(Tesseract_DIR "D:/dev/vcpkg/installed/x64-windows-static/share/tesseract")

#set(CURL_INCLUDE_DIR "D:/dev/vcpkg/installed/x64-windows/include")
#set(CURL_LIBRARY_RELEASE "D:/dev/vcpkg/installed/x64-windows/lib/libcurl.lib")
#set(Leptonica_DIR "D:/dev/vcpkg/installed/x64-windows/share/leptonica")
#set(OpenJPEG_DIR "D:/dev/vcpkg/installed/x64-windows/share/openjpeg")
#set(WebP_DIR "D:/dev/vcpkg/installed/x64-windows/share/webp")
#set(LibArchive_INCLUDE_DIR "D:/dev/vcpkg/installed/x64-windows/include")
#set(LibArchive_LIBRARY "D:/dev/vcpkg/installed/x64-windows/lib/libarchive.lib")

project(videoEditor LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Multimedia MultimediaWidgets)
find_package(OpenCV REQUIRED)

find_package(Tesseract CONFIG REQUIRED)

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

        video/clip/ClipBase.h
        video/clip/ImageClip.h
        video/clip/AudioClip.h
        video/clip/VideoClip.h
        video/clip/TextClip.h
        video/render/VideoRender.cpp
        video/render/VideoRender.h
        video/preview/ClipsPreview.cpp
        video/preview/ClipsPreview.h
        video/clip/VideoClip.cpp
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

        icon.qrc
        icon.rc

        image/imageocr.cpp
        image/imageocr.h
)

target_include_directories(videoEditor
        PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${TESSERACT_INCLUDE_DIR}
)

target_link_libraries(videoEditor
        PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Multimedia
        Qt::MultimediaWidgets
        ${OpenCV_LIBS}
        Tesseract::libtesseract
)

include(GNUInstallDirs)

install(TARGETS videoEditor
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(WIN32)
    # 创建Windows图标资源文件
    configure_file(icon.rc @ONLY)
    set(WIN32_RESOURCE_FILES icon.rc)
else()
    set(WIN32_RESOURCE_FILES "")
endif()

qt_generate_deploy_app_script(
        TARGET videoEditor
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
