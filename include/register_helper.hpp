// register_hepler.hpp : helper functions for registration
// Austin Hester CS642o feb 2021
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#ifndef REGISTER_HELPER_H
#define REGISTER_HELPER_H

#include <opencv2/core/core.hpp>

#include <iostream>


struct ManualState {
    std::string* windowName;
    cv::Mat* imageWithPoints;
    int maxPoints;
    std::vector<cv::Point> points;
};

void
initialize_images(
    const std::string imageFilename,
    const std::string templateFilename,
    cv::Mat* inputImage,
    cv::Mat* equalGrayInputImage,
    cv::Mat* equalTemplateImage
);

void mouse_callback_pick_points(int event, int x, int y, int d, void* userdata);
ManualState initialize_callback(std::string* windowName, cv::Mat* image, int maxPoints);

#endif
