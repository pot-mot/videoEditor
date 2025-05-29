cmake_minimum_required(VERSION 3.19)
project(videoEditor LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(OpenCV REQUIRED)

message(STATUS "OpenCV_VERSION: ${OpenCV_VERSION}")
message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")

qt_standard_project_setup()

qt_add_executable(videoEditor
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    imageprocessor.h imageprocessor.cpp
    commandhistory.h commandhistory.cpp
    imageeditor.h imageeditor.cpp imageeditor.ui
    videoeditor.h videoeditor.cpp videoeditor.ui
)

target_include_directories(videoEditor
    PRIVATE
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(videoEditor
    PRIVATE
        Qt::Core
        Qt::Widgets
        ${OpenCV_LIBS}
)

include(GNUInstallDirs)

install(TARGETS videoEditor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET videoEditor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
