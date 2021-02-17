// register_hepler.cpp : helper functions for registration
// Austin Hester CS642o feb 2021
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// #include <opencv2/video/tracking.hpp>

#include <iostream>

#include "./include/dir_func.hpp"
#include "./include/register_helper.hpp"


void
initialize_images(
    const std::string image_filename,
    const std::string template_filename,
    cv::Mat* input_image,
    cv::Mat* equal_gray_input_image,
    cv::Mat* equal_template_image
) {
    // open image as color
    *input_image = open_image(image_filename.c_str(), false);

    // convert input image to grayscale
    cv::cvtColor(*input_image, *equal_gray_input_image, cv::COLOR_BGR2GRAY);
    // equalize grayscale input image
    cv::equalizeHist(*equal_gray_input_image, *equal_gray_input_image);

    // read template image as grayscale
    *equal_template_image = open_image(template_filename.c_str(), true);
    // equalize template input image
    cv::equalizeHist(*equal_template_image, *equal_template_image);
}


void
mouse_callback_pick_points(int event, int x, int y, int d, void* userdata)
{
    ManualState* state = (ManualState*) userdata;

    switch (event) {
        case cv::EVENT_LBUTTONUP:
            // limit amount of points
            if (state->points.size() >= state->max_points) break;
            // push the new point
            state->points.push_back(cv::Point(x, y));
            // draw a circle mask at chosen points
            cv::Mat circle_mask = cv::Mat::zeros(state->image->size(), CV_8UC1);
            cv::circle(circle_mask, cv::Point(x, y), 5, cv::Scalar(255), cv::FILLED);
            // make circle more contrasty by inverting and drawing black border
            cv::bitwise_not(*(state->image), *(state->image), circle_mask);
            cv::circle(*(state->image), cv::Point(x, y), 5, cv::Scalar(0));
            // show the copy of the image
            cv::imshow(*(state->window_name), *(state->image));
            break;
    }
}


// assign mouse callbacks
void
init_callback(ManualState* state, std::string* window_name, cv::Mat* image, int max_points)
{
    *state = { window_name, image, max_points };
    cv::setMouseCallback( *window_name, mouse_callback_pick_points, state );
}
