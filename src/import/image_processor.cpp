#include "image_processor.h"

#include <algorithm>
#include <array>
#include <vector>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace
{
constexpr int k_page_width = 1800;
constexpr int k_page_height = 2400;
constexpr double k_minimum_page_area_ratio = 0.20;

std::array<cv::Point2f, 4> order_corners(const std::vector<cv::Point> &points)
{
    std::array<cv::Point2f, 4> ordered {};
    std::vector<cv::Point2f> corners;
    for (const cv::Point &point : points) {
        corners.emplace_back(static_cast<float>(point.x), static_cast<float>(point.y));
    }
    auto sum=[](const cv::Point2f &p){return p.x+p.y;};
    auto diff=[](const cv::Point2f &p){return p.y-p.x;};
    ordered[0]=*std::min_element(corners.begin(),corners.end(),[&](const auto&a,const auto&b){return sum(a)<sum(b);});
    ordered[2]=*std::max_element(corners.begin(),corners.end(),[&](const auto&a,const auto&b){return sum(a)<sum(b);});
    ordered[1]=*std::min_element(corners.begin(),corners.end(),[&](const auto&a,const auto&b){return diff(a)<diff(b);});
    ordered[3]=*std::max_element(corners.begin(),corners.end(),[&](const auto&a,const auto&b){return diff(a)<diff(b);});
    return ordered;
}
}

QImage Image_Processor::to_qimage(const cv::Mat &image) const
{
    if (image.empty()) return {};
    if (image.type()==CV_8UC1) return QImage(image.data,image.cols,image.rows,static_cast<qsizetype>(image.step),QImage::Format_Grayscale8).copy();
    if (image.type()==CV_8UC3) {
        cv::Mat rgb_image; cv::cvtColor(image,rgb_image,cv::COLOR_BGR2RGB);
        return QImage(rgb_image.data,rgb_image.cols,rgb_image.rows,static_cast<qsizetype>(rgb_image.step),QImage::Format_RGB888).copy();
    }
    if (image.type()==CV_8UC4) {
        cv::Mat rgba_image; cv::cvtColor(image,rgba_image,cv::COLOR_BGRA2RGBA);
        return QImage(rgba_image.data,rgba_image.cols,rgba_image.rows,static_cast<qsizetype>(rgba_image.step),QImage::Format_RGBA8888).copy();
    }
    return {};
}

bool Image_Processor::clean_page(const QString &input_path,const QString &output_path,QString *error) const
{
    const cv::Mat source=cv::imread(input_path.toStdString(),cv::IMREAD_COLOR);
    if(source.empty()){if(error)*error="OpenCV could not decode the image.";return false;}
    cv::Mat page=detect_and_warp_page(source);
    if(page.empty()) page=source;
    const cv::Mat normalized=normalize_page(page);
    if(normalized.empty()){if(error)*error="Image normalization failed.";return false;}
    if(!cv::imwrite(output_path.toStdString(),normalized)){if(error)*error="Could not write the canonical PNG.";return false;}
    return true;
}

cv::Mat Image_Processor::detect_and_warp_page(const cv::Mat &source) const
{
    cv::Mat gray; cv::cvtColor(source,gray,cv::COLOR_BGR2GRAY); cv::GaussianBlur(gray,gray,cv::Size(5,5),0.0);
    cv::Mat edges; cv::Canny(gray,edges,60.0,180.0);
    std::vector<std::vector<cv::Point>> contours; cv::findContours(edges,contours,cv::RETR_LIST,cv::CHAIN_APPROX_SIMPLE);
    const double minimum_area=static_cast<double>(source.total())*k_minimum_page_area_ratio;
    double best_area=0.0; std::vector<cv::Point> best_quad;
    for(const auto &contour:contours){
        const double perimeter=cv::arcLength(contour,true); std::vector<cv::Point> approximation;
        cv::approxPolyDP(contour,approximation,0.02*perimeter,true);
        if(approximation.size()!=4 || !cv::isContourConvex(approximation)) continue;
        const double area=std::abs(cv::contourArea(approximation));
        if(area>=minimum_area && area>best_area){best_area=area;best_quad=approximation;}
    }
    if(best_quad.empty()) return {};
    const auto source_points=order_corners(best_quad);
    const std::array<cv::Point2f,4> destination_points={
        cv::Point2f(0,0),cv::Point2f(k_page_width-1,0),
        cv::Point2f(k_page_width-1,k_page_height-1),cv::Point2f(0,k_page_height-1)};
    const cv::Mat transform=cv::getPerspectiveTransform(source_points.data(),destination_points.data());
    cv::Mat warped; cv::warpPerspective(source,warped,transform,cv::Size(k_page_width,k_page_height),cv::INTER_CUBIC,cv::BORDER_REPLICATE);
    return warped;
}

cv::Mat Image_Processor::normalize_page(const cv::Mat &page) const
{
    cv::Mat resized; cv::resize(page,resized,cv::Size(k_page_width,k_page_height),0,0,cv::INTER_AREA);
    cv::Mat lab; cv::cvtColor(resized,lab,cv::COLOR_BGR2Lab);
    std::vector<cv::Mat> channels; cv::split(lab,channels);
    cv::Ptr<cv::CLAHE> clahe=cv::createCLAHE(2.0,cv::Size(8,8)); clahe->apply(channels[0],channels[0]);
    cv::merge(channels,lab); cv::Mat normalized; cv::cvtColor(lab,normalized,cv::COLOR_Lab2BGR);
    return normalized;
}
