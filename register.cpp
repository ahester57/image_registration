// register.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <iostream>

#include "./include/cla_parse.hpp"


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
    bool manual;
    float epsilon;

    // parse and save command line args
    int parse_result = parse_arguments(
        argc, argv,
        &imageFilename,
        &templateFilename,
        &warpFilename,
        &manual,
        &epsilon
    );
    if (parse_result != 1) return parse_result;

    std::cout << "\nShortcuts:\n\tq\t- quit\n";

    // image registration

    // 'event loop' for keypresses
    while (wait_key());

    return 0;
}
