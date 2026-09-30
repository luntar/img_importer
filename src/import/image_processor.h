#pragma once

#include <QImage>

#include <opencv2/core/mat.hpp>

class Image_Processor
{
public:
    Image_Processor() = default;

    /**
     * @brief Converts an OpenCV image into a Qt image.
     * @param image Source OpenCV image.
     * @return Detached QImage containing the converted pixels.
     */
    [[nodiscard]] QImage to_qimage(const cv::Mat &image) const;
};
