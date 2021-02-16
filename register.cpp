// register.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>

#include <iostream>
#include <vector>

#include "./include/cla_parse.hpp"
#include "./include/dir_func.hpp"


const std::string WINDOW_NAME = "Image Registration";


struct ManualState {
    std::string* windowName;
    cv::Mat* image;
    cv::Mat* imageWithPoints;
    std::vector<cv::Point> points;
};

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
mouse_callback(int event, int x, int y, int d, void* userdata)
{
    ManualState* state = (ManualState*) userdata;

    switch (event) {
        case cv::EVENT_LBUTTONUP:
            state->points.push_back(cv::Point(x, y));
            // std::cout << "-----" << std::endl;
            // for (auto p : state->points) {
            //     std::cout << p << "\t";
            // }
            // std::cout << std::endl;
            // draw a circle on the image to show where they've clicked
            cv::circle(
                *(state->imageWithPoints),
                cv::Point(x, y), 5,
                cv::Scalar(255),
                cv::FILLED, cv::LINE_8
            );
            cv::imshow(*(state->windowName), *(state->imageWithPoints));
            break;
    }
}


int
main(int argc, const char** argv)
{
    // CLA variables
    std::string imageFilename;
    std::string templateFilename;
    std::string warpFilename;
    bool manual;
    float epsilon;
    int motionType;
    std::string outputWarp;
    std::string warpImgFilename;

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

    initialize_images(
        imageFilename,
        templateFilename,
        &inputImage,
        &equalGrayInputImage,
        &equalTemplateImage
    );

    std::cout << "\nShortcuts:\n\tq\t- quit\n";

    // image registration
    cv::imshow(WINDOW_NAME + " Input Image", inputImage);    while (wait_key());

    std::string equalGrayTitle = WINDOW_NAME + " Equalized Grayscale Image";
    cv::imshow(equalGrayTitle, equalGrayInputImage);

    std::string equalTemplateTitle = WINDOW_NAME + " Template Image";
    cv::imshow(equalTemplateTitle, equalTemplateImage);

    cv::Mat inputImageWithPoints;
    cv::Mat templateImageWithPoints;

    if (manual) {
        equalGrayInputImage.copyTo(inputImageWithPoints);
        ManualState inputState = { &equalGrayTitle, &equalGrayInputImage, &inputImageWithPoints };

        equalTemplateImage.copyTo(templateImageWithPoints);
        ManualState templateState = { &equalTemplateTitle, &equalTemplateImage, &templateImageWithPoints };

        cv::setMouseCallback(equalGrayTitle, mouse_callback, &inputState);
        cv::setMouseCallback(equalTemplateTitle, mouse_callback, &templateState);
    }

    // 'event loop' for keypresses
    while (wait_key());

    inputImage.release();
    equalGrayInputImage.release();
    equalTemplateImage.release();
    inputImageWithPoints.release();
    templateImageWithPoints.release();

    return 0;
}
