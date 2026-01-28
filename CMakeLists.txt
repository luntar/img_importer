cmake_minimum_required(VERSION 3.16)

project(pdf_to_images LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Pdf)

qt_standard_project_setup()

add_executable(pdf_to_images
    src/main.cpp
)

target_link_libraries(pdf_to_images
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Pdf
)
