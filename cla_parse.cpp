// cla_parse.cpp : Parse given command line arguments.
// Austin Hester CS542o nov 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>

#include "./include/cla_parse.hpp"


// parse command line arguments
int
parse_arguments(
    int argc,
    const char** argv,
    std::string* image_filename,
    std::string* template_filename,
    std::string* warp_filename,
    bool*        manual,
    double*      epsilon,
    std::string* motion_type,
    std::string* output_warp,
    std::string* warp_img_filename
) {
    cv::String keys =
        "{@imagefile      |<none>         | Input Image}"
        "{@templatefile   |<none>         | Template Image}"
        "{@warpfile       |<none>         | Warp Matrix}"
        "{manual M        |               | Perform Manual Registration}"
        "{epsilon e       |0.0001         | ECC's Convergence Epsilon}"
        "{motion_type m   |affine         | Type of Motion: translation/euclidean/affine/homography}"
        "{output_warp o   |out_warp.ecc   | Output warp matrix filename}"
        "{warp_img_file w |warped_ecc.jpg | Output warp image}"
        "{help h          |               | Show Help Message}";

    cv::CommandLineParser parser(argc, argv, keys);

    if (parser.has("h")) {
        parser.printMessage();
        return 0;
    }

    if (!parser.check()) {
        parser.printErrors();
        parser.printMessage();
        return -1;
    }

    try {
        *image_filename = (std::string) parser.get<std::string>(0).c_str();
        assert(image_filename->size() > 0);
    } catch (...) {
        std::cerr << "Failed to parse imagefile argument!:" << std::endl;
        return -1;
    }

    try {
        *template_filename = (std::string) parser.get<std::string>(1).c_str();
        assert(template_filename->size() > 0);
    } catch (...) {
        std::cerr << "Failed to parse templatefile argument!:" << std::endl;
        return -1;
    }

    try {
        *warp_filename = (std::string) parser.get<std::string>(2).c_str();
    } catch (...) {
        std::cerr << "Failed to parse warpfile argument!:" << std::endl;
        return -1;
    }

    try {
        *manual = (bool) parser.has("M");
        assert((warp_filename->size() > 0) != *manual || ((warp_filename->size() == 0) && !*manual));
    } catch (...) {
        std::cerr << "Failed to parse manual argument." << std::endl;
        return -1;
    }

    try {
        *epsilon = parser.get<float>("e");
        assert(*epsilon > 0 && *epsilon <= 0.1);
    } catch (...) {
        std::cerr << "Failed to parse epsilon argument!:" << std::endl;
        return -1;
    }

    try {
        *motion_type = parser.get<std::string>("m").c_str();
    } catch (...) {
        std::cerr << "Failed to parse motion_type argument!:" << std::endl;
        return -1;
    }

    try {
        *output_warp = (std::string) parser.get<std::string>("o").c_str();
    } catch (...) {
        std::cerr << "Failed to parse output_warp argument!:" << std::endl;
        return -1;
    }

    try {
        *warp_img_filename = (std::string) parser.get<std::string>("w").c_str();
    } catch (...) {
        std::cerr << "Failed to parse warp_img_filename argument!:" << std::endl;
        return -1;
    }

    std::cout << std::endl << "Shortcuts:" << std::endl << "\tq\t- quit" << std::endl << std::endl;

    return 1;
}
