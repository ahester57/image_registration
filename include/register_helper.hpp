// register_hepler.hpp : helper functions for registration
// Austin Hester CS642o feb 2021
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#ifndef REGISTER_HELPER_H
#define REGISTER_HELPER_H

#include <opencv2/core/core.hpp>

#include <iostream>


struct ManualState {
    std::string* window_name;
    cv::Mat* image;
    int max_points;
    std::vector<cv::Point2f> points;
};

void
initialize_images(
    const std::string image_filename,
    const std::string template_filename,
    cv::Mat* input_image,
    cv::Mat* equal_gray_input_image,
    cv::Mat* template_image,
    cv::Mat* equal_template_image
);

void mouse_callback_pick_points(int event, int x, int y, int d, void* userdata);
void init_callback(ManualState* state, std::string* window_name, cv::Mat* image, int max_points);
int motion_type_string_to_int(std::string motion_type_string);
void print_warp_matrix(cv::Mat warp_matrix, bool original = false);
void print_results(std::string motion_type, double correlation_co);
void create_affine_warp_matrix(ManualState state_1, ManualState state_2, cv::Mat* warp_matrix);

#endif
