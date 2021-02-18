// cla_parse.hpp : Parse given command line arguments.
// Austin Hester CS542o sept 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#ifndef CLA_PARSE_H
#define CLA_PARSE_H

#include <opencv2/core/core.hpp>
#include <iostream>

int parse_arguments(
    int argc,
    const char** argv,
    std::string* image_filename,
    std::string* template_filename,
    std::string* warp_filename,
    bool*        manual,
    double*      epsilon,
    int*         motion_type,
    std::string* output_warp,
    std::string* warp_img_filename
);

#endif
