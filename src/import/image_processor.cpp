#include "image_processor.h"

#include <opencv2/imgproc.hpp>

QImage Image_Processor::to_qimage(const cv::Mat &image) const
{
    if (image.empty()) {
        return {};
    }

    if (image.type() == CV_8UC1) {
        return QImage(
            image.data,
            image.cols,
            image.rows,
            static_cast<qsizetype>(image.step),
            QImage::Format_Grayscale8).copy();
    }

    if (image.type() == CV_8UC3) {
        cv::Mat rgb_image;
        cv::cvtColor(image, rgb_image, cv::COLOR_BGR2RGB);

        return QImage(
            rgb_image.data,
            rgb_image.cols,
            rgb_image.rows,
            static_cast<qsizetype>(rgb_image.step),
            QImage::Format_RGB888).copy();
    }

    if (image.type() == CV_8UC4) {
        cv::Mat rgba_image;
        cv::cvtColor(image, rgba_image, cv::COLOR_BGRA2RGBA);

        return QImage(
            rgba_image.data,
            rgba_image.cols,
            rgba_image.rows,
            static_cast<qsizetype>(rgba_image.step),
            QImage::Format_RGBA8888).copy();
    }

    return {};
}
