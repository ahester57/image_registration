// register.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS642o feb 2021
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include <iostream>
#include <vector>

#include "./include/cla_parse.hpp"
#include "./include/register_helper.hpp"


const std::string WINDOW_NAME = "Image Registration";


// 'event loop' for keypresses
int
wait_key()
{
    char key_pressed = cv::waitKey(0) & 255;
    if (cv::getWindowProperty(WINDOW_NAME, cv::WND_PROP_VISIBLE) < 1) {
        // this ends the program if window is closed
        return 0;
    }
    // 'q' or  <escape> quits out
    if (key_pressed == 27 || key_pressed == 'q') {
        cv::destroyAllWindows();
        return 0;
    }
    return 1;
}


int
main(int argc, const char** argv)
{
    // CLA variables
    std::string imageFilename;
    std::string templateFilename;
    std::string warpFilename;
    std::string outputWarp;
    std::string warpImgFilename;
    bool  manual;
    float epsilon;
    int   motionType;

    // parse and save command line args
    int parse_result = parse_arguments(
        argc, argv,
        &imageFilename,
        &templateFilename,
        &warpFilename,
        &manual,
        &epsilon,
        &motionType,
        &outputWarp,
        &warpImgFilename
    );
    if (parse_result != 1) return parse_result;


    // initialize images
    cv::Mat inputImage;
    cv::Mat equalGrayInputImage;
    cv::Mat equalTemplateImage;
    cv::Mat inputImageWithPoints;       // } only for
    cv::Mat templateImageWithPoints;    // } manual registration

    initialize_images(
        imageFilename,
        templateFilename,
        &inputImage,
        &equalGrayInputImage,
        &equalTemplateImage
    );

    // set max points for warp matrix
    int max_points = motionType != cv::MOTION_HOMOGRAPHY ? 6 : 9;

    // begin image registration by displaying input
    cv::imshow( WINDOW_NAME + " Input Image", inputImage );
    while (wait_key());

    std::string equalGrayTitle = WINDOW_NAME + " Equalized Grayscale Image";
    cv::imshow( equalGrayTitle, equalGrayInputImage  );
    if (manual) {
        // create manual state for input image
        equalGrayInputImage.copyTo(inputImageWithPoints);
        initialize_callback(&equalGrayTitle, &inputImageWithPoints, max_points);
    }

    std::string equalTemplateTitle = WINDOW_NAME + " Template Image";
    cv::imshow( equalTemplateTitle, equalTemplateImage );
    if (manual) {
        // create manual state for template image
        equalTemplateImage.copyTo(templateImageWithPoints);
        initialize_callback(&equalTemplateTitle, &templateImageWithPoints, max_points);
    }

    // create warp matrix

    // 'event loop' for keypresses
    while (wait_key());

    inputImage.release();
    equalGrayInputImage.release();
    equalTemplateImage.release();
    inputImageWithPoints.release();
    templateImageWithPoints.release();

    return 0;
}
