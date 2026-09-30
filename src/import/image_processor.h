#pragma once

#include <QImage>
#include <QString>

#include <opencv2/core/mat.hpp>

class Image_Processor
{
public:
    Image_Processor() = default;

    /** @brief Converts an OpenCV image into a Qt image. */
    [[nodiscard]] QImage to_qimage(const cv::Mat &image) const;

    /**
     * @brief Cleans a photographed score page and writes a canonical PNG.
     * @param input_path Source image path.
     * @param output_path Destination PNG path.
     * @param error Receives a human-readable error on failure.
     * @return True when a canonical page was written.
     */
    [[nodiscard]] bool clean_page(const QString &input_path, const QString &output_path, QString *error) const;

private:
    [[nodiscard]] cv::Mat detect_and_warp_page(const cv::Mat &source) const;
    [[nodiscard]] cv::Mat normalize_page(const cv::Mat &page) const;
};
