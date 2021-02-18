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
    // 'q' or  <escape> quits out
    if (key_pressed == 27 || key_pressed == 'q') {
        return 0;
    }
    return 1;
}


int
wait_state_full(ManualState state)
{
    char key_pressed = cv::waitKey(66) & 255;
    return !(state.points.size() >= state.max_points);
}


int
main(int argc, const char** argv)
{
    // CLA variables
    std::string image_filename;
    std::string template_filename;
    std::string warp_filename;
    std::string output_warp;
    std::string warp_img_filename;
    bool   manual;
    double epsilon;
    int    motion_type;

    // parse and save command line args
    int parse_result = parse_arguments(
        argc, argv,
        &image_filename,
        &template_filename,
        &warp_filename,
        &manual,
        &epsilon,
        &motion_type,
        &output_warp,
        &warp_img_filename
    );
    if (parse_result != 1) return parse_result;


    // initialize images
    cv::Mat input_image;
    cv::Mat equal_gray_input_image;
    cv::Mat equal_template_image;
    cv::Mat input_image_copy;       // } only for
    cv::Mat template_image_copy;    // } manual registration

    initialize_images(
        image_filename,
        template_filename,
        &input_image,
        &equal_gray_input_image,
        &equal_template_image
    );

    // set max points for warp matrix
    int max_points = motion_type != cv::MOTION_HOMOGRAPHY ? 6 : 9;

    // begin image registration by displaying input
    cv::imshow( WINDOW_NAME + " Input Image", input_image );
    wait_key(); // flash original until input

    // Template Image
    std::string template_subtitle = " Template Image";
    std::string template_title = WINDOW_NAME + template_subtitle;
    cv::imshow( template_title, equal_template_image );

    // Equalized, Grayscale Input Image
    std::string equal_gray_subtitle = " Equalized Grayscale Image";
    std::string equal_gray_title = WINDOW_NAME + equal_gray_subtitle;
    cv::imshow( equal_gray_title, equal_gray_input_image  );

    // create warp matrix
    cv::Mat warp_matrix;
    // i hate size for being opposite
    cv::Size warp_matrix_size = cv::Size(3, motion_type != cv::MOTION_HOMOGRAPHY ? 2 : 3);

    if (manual) {
        // create manual state for input image
        ManualState input_state;
        equal_gray_input_image.copyTo(input_image_copy);
        init_callback(&input_state, &equal_gray_title, &input_image_copy, max_points);
        // wait for them to pick all the points on the image.
        std::cout << "Manual Mode: Please choose all points on " << equal_gray_subtitle << std::endl;
        while (wait_state_full(input_state));

        // create manual state for template image
        ManualState template_state;
        equal_template_image.copyTo(template_image_copy);
        init_callback(&template_state, &template_title, &template_image_copy, max_points);
        // wait for them to pick all the points on the image.
        std::cout << "Manual Mode: Please choose all points on " << template_subtitle << std::endl;
        while (wait_state_full(template_state));
        // use points to initialize warp matrix
    }

    // } elif (warp_filename.size() > 0) {
    //     // TODO warp matrix read in file
    // }

    if (warp_matrix.size().area() == 0) {
        // if warp_matrix, hasn't been set yet, use identity matrix
        warp_matrix = cv::Mat::eye(warp_matrix_size, CV_32F);
    }
    // findTransformECC returns the final enhanced correlation coefficient,
    // that is the correlation coefficient between the template image and the final warped input image.
    double correlation_co
    = cv::findTransformECC(
        equal_template_image,
        equal_gray_input_image,
        warp_matrix,
        motion_type,
        cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, warp_matrix.size().area(), epsilon)
    );

    std::cout << correlation_co;
    // 'event loop' for keypresses
    while (wait_key());

    cv::destroyAllWindows();
    input_image.release();
    equal_gray_input_image.release();
    equal_template_image.release();
    input_image_copy.release();
    template_image_copy.release();

    return 0;
}
