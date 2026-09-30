cmake_minimum_required(VERSION 3.21)

project(img_importer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Quick Qml QuickDialogs2)
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(img_importer
    src/main.cpp
    src/app/window_state_manager.cpp
    src/app/window_state_manager.h
    src/import/image_processor.cpp
    src/import/image_processor.h
)

qt_add_qml_module(img_importer
    URI ImgImporter
    VERSION 1.0
    QML_FILES
        src/ui/Main.qml
)

target_include_directories(img_importer PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    src
)

target_link_libraries(img_importer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::QuickDialogs2
    ${OpenCV_LIBS}
)
