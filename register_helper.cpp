// register_hepler.cpp : helper functions for registration
// Austin Hester CS642o feb 2021
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include <iostream>
#include <map>

#include "./include/dir_func.hpp"
#include "./include/register_helper.hpp"


void
initialize_images(
    const std::string image_filename,
    const std::string template_filename,
    cv::Mat* input_image,
    cv::Mat* equal_gray_input_image,
    cv::Mat* template_image,
    cv::Mat* equal_template_image
) {
    // open image as color
    *input_image = open_image(image_filename.c_str(), false);

    // convert input image to grayscale
    cv::cvtColor(*input_image, *equal_gray_input_image, cv::COLOR_BGR2GRAY);
    // equalize grayscale input image
    cv::equalizeHist(*equal_gray_input_image, *equal_gray_input_image);

    // read template image as grayscale
    *template_image = open_image(template_filename.c_str(), true);
    // equalize template input image
    cv::equalizeHist(*template_image, *equal_template_image);
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
            state->points.push_back(cv::Point2f(x, y));
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

// convert given motion type to enum int
int
motion_type_string_to_int(std::string motion_type_string)
{
    std::map<std::string, int> motion_type_map = {
        { "translation", cv::MOTION_TRANSLATION },
        { "euclidean"  , cv::MOTION_EUCLIDEAN   },
        { "affine"     , cv::MOTION_AFFINE      },
        { "homography" , cv::MOTION_HOMOGRAPHY  }
    };
    int motion_type = motion_type_map[motion_type_string];
    // assertion to prevent them from typing wrong and getting translation
    assert(motion_type_string != "translation" ? motion_type != 0 : true);
    return motion_type;
}


void
print_warp_matrix(cv::Mat warp_matrix, bool original)
{
    std::cout << std::endl
            << (original ? "Original " : "Transformed ")
            << "Warp Matrix: " << std::endl;
    for (int i = 0; i < warp_matrix.size().height; i++) {
        std::cout << "[";
        for (int j = 0; j < warp_matrix.size().width-1; j++) {
            std::cout << warp_matrix.at<float>(i, j) << ",\t";
        }
        std::cout << warp_matrix.at<float>(i, warp_matrix.size().width-1) << "]" << std::endl;
    }
}


void
print_results(std::string motion_type, double correlation_co)
{
    std::cout << std::endl << "Motion Type: " << motion_type << std::endl;
    std::cout << std::endl << "Correlation Coefficient: " << correlation_co << std::endl;
}


void
create_affine_warp_matrix(ManualState state_1, ManualState state_2, cv::Mat* warp_matrix)
{
    // & <vector> [0] converts vector to primitive array
    cv::getAffineTransform(&state_1.points[0], &state_2.points[0]).convertTo(*warp_matrix, CV_32F);
}

void
create_homography_warp_matrix(ManualState state_1, ManualState state_2, cv::Mat* warp_matrix)
{
    // & <vector> [0] converts vector to primitive array
    cv::getPerspectiveTransform(&state_1.points[0], &state_2.points[0]).convertTo(*warp_matrix, CV_32F);
}
