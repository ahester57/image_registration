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
    const std::string imageFilename,
    const std::string templateFilename,
    cv::Mat* inputImage,
    cv::Mat* equalGrayInputImage,
    cv::Mat* equalTemplateImage
) {
    // open image as color
    *inputImage = open_image(imageFilename.c_str(), false);

    // convert input image to grayscale
    cv::cvtColor(*inputImage, *equalGrayInputImage, cv::COLOR_BGR2GRAY);
    // equalize grayscale input image
    cv::equalizeHist(*equalGrayInputImage, *equalGrayInputImage);

    // read template image as grayscale
    *equalTemplateImage = open_image(templateFilename.c_str(), true);
    // equalize template input image
    cv::equalizeHist(*equalTemplateImage, *equalTemplateImage);
}


void
mouse_callback_pick_points(int event, int x, int y, int d, void* userdata)
{
    ManualState* state = (ManualState*) userdata;

    switch (event) {
        case cv::EVENT_LBUTTONUP:
            // limit amount of points
            if (state->points.size() >= state->maxPoints) break;
            // push the new point
            state->points.push_back(cv::Point(x, y));
            // draw a circle on a copy of the image
            cv::circle(
                *(state->imageWithPoints),
                cv::Point(x, y), 5,
                cv::Scalar(255),
                cv::FILLED, cv::LINE_8
            );
            // show the copy of the image
            cv::imshow(*(state->windowName), *(state->imageWithPoints));
            break;
    }
}


// assign mouse callbacks
ManualState
initialize_callback(std::string* windowName, cv::Mat* image, int maxPoints)
{
    ManualState manualState = { windowName, image, maxPoints };
    cv::setMouseCallback( *windowName, mouse_callback_pick_points, &manualState );
    return manualState;
}
