// register_hepler.hpp : helper functions for registration
// Austin Hester CS642o feb 2021
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#ifndef REGISTER_HELPER_H
#define REGISTER_HELPER_H

#include <opencv2/core/core.hpp>

#include <iostream>


struct ManualState {
    std::string* window_name;
    cv::Mat* image_with_points;
    int max_points;
    std::vector<cv::Point> points;
};

void
initialize_images(
    const std::string image_filename,
    const std::string template_filename,
    cv::Mat* input_image,
    cv::Mat* equal_gray_input_image,
    cv::Mat* equal_template_image
);

void mouse_callback_pick_points(int event, int x, int y, int d, void* userdata);
ManualState initialize_callback(std::string* window_name, cv::Mat* image, int max_points);

#endif
