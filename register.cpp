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
#include "./include/dir_func.hpp"
#include "./include/register_helper.hpp"

#define DEBUG 1

const std::string WINDOW_NAME = "Image Registration";


// 'event loop' for keypresses
// call in a while loop to only register q or <esc>
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


// wait until the number of points in state
// reaches the maximum allowed
void
wait_state_full(ManualState* state)
{
    while (1) {
        char key_pressed = cv::waitKey(66) & 255;
        if (state->points.size() >= state->max_points) {
            return;
        }
    }
    return;
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
    std::string motion_type_string;
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
        &motion_type_string,
        &output_warp,
        &warp_img_filename
    );
    if (parse_result != 1) return parse_result;

    motion_type = motion_type_string_to_int(motion_type_string);

    // initialize images
    cv::Mat input_image;
    cv::Mat equal_gray_input_image;
    cv::Mat template_image;
    cv::Mat equal_template_image;
    cv::Mat input_image_copy;       // } only for
    cv::Mat template_image_copy;    // } manual registration

    initialize_images(
        image_filename,
        template_filename,
        &input_image,
        &equal_gray_input_image,
        &template_image,
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

    // i hate size for being opposite
    cv::Size warp_matrix_size = cv::Size(3, motion_type != cv::MOTION_HOMOGRAPHY ? 2 : 3);
    // create warp matrix
    cv::Mat warp_matrix = cv::Mat::eye(warp_matrix_size, CV_32F);;

    // set warp matrix manually
    if (manual) {
        // create manual state for input image
        ManualState input_state;
        equal_gray_input_image.copyTo(input_image_copy);
        init_callback( &input_state, &equal_gray_title, &input_image_copy, max_points );
        // wait for them to pick all the points on the image.
        std::cout << "Manual Mode: Please choose all points on " << equal_gray_subtitle << std::endl;
        wait_state_full( &input_state );

        // create manual state for template image
        ManualState template_state;
        equal_template_image.copyTo(template_image_copy);
        init_callback( &template_state, &template_title, &template_image_copy, max_points );
        // wait for them to pick all the points on the image.
        std::cout << "Manual Mode: Please choose all points on " << template_subtitle << std::endl;
        wait_state_full( &template_state );

        // use homogeneous points to initialize warp matrix
        motion_type != cv::MOTION_HOMOGRAPHY ?
            create_affine_warp_matrix( input_state, template_state, &warp_matrix ) :
            create_homography_warp_matrix( input_state, template_state, &warp_matrix );

        input_image_copy.release();
        template_image_copy.release();

    } else if (warp_filename.size() > 0) {
        // TODO warp matrix read in file
        open_image_from_text(warp_filename, &warp_matrix);
    }

    // display input warp matrix
    print_warp_matrix(warp_matrix, true);

    // findTransformECC returns the final enhanced correlation coefficient,
    // that is the correlation coefficient between the template image and the final warped input image.
    double correlation_co
        = cv::findTransformECC(
            equal_template_image,
            equal_gray_input_image,
            warp_matrix,
            motion_type,
            cv::TermCriteria(
                cv::TermCriteria::COUNT+cv::TermCriteria::EPS,
                warp_matrix.size().area(),
                epsilon)
        );


    // warp original image using transformed warp matrix
    motion_type != cv::MOTION_HOMOGRAPHY ?
        cv::warpAffine(input_image, input_image_copy, warp_matrix, input_image.size()) :
        cv::warpPerspective(input_image, input_image_copy, warp_matrix, input_image.size());


    print_warp_matrix(warp_matrix);
    print_results(motion_type_string, correlation_co);
    write_img_to_file_as_text(warp_matrix, "./out", motion_type_string + "_" + output_warp);


    cv::imshow( WINDOW_NAME + " Warped", input_image_copy  );
    write_img_to_file(input_image_copy, "./out", motion_type_string + "_" + warp_img_filename);

    // 'event loop' for keypresses
    wait_key();

    // get grayscale of input image
    cv::Mat gray_warped;
    cv::cvtColor(input_image_copy, gray_warped, cv::COLOR_BGR2GRAY);

    // compute and show error
    cv::Mat error = gray_warped - template_image;

    double minVal;
    double maxVal;
    cv::Point minPoint;
    cv::Point maxPoint;
    // get max error
    cv::minMaxLoc(error, &minVal, &maxVal, &minPoint, &maxPoint);
    // scale error image
    cv::Mat scaled_error = cv::abs(error) * (255/maxVal);
    // and finally show it
    cv::imshow( WINDOW_NAME + " Error", scaled_error );

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
