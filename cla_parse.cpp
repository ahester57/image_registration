// cla_parse.cpp : Parse given command line arguments.
// Austin Hester CS542o nov 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>

#include <map>

#include "./include/cla_parse.hpp"

// parse command line arguments
int
parse_arguments(
    int argc,
    const char** argv,
    std::string* imageFilename,
    std::string* templateFilename,
    std::string* warpFilename,
    bool* manual,
    float* epsilon,
    int* motionType,
    std::string* outputWarp,
    std::string* warpImgFilename
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
        *imageFilename = (std::string) parser.get<std::string>(0).c_str();
        assert(imageFilename->size() > 0);
    } catch (...) {
        std::cerr << "Failed to parse imagefile argument!:" << std::endl;
        return -1;
    }

    try {
        *templateFilename = (std::string) parser.get<std::string>(1).c_str();
    } catch (...) {
        std::cerr << "Failed to parse templatefile argument!:" << std::endl;
        return -1;
    }

    try {
        *warpFilename = (std::string) parser.get<std::string>(2).c_str();
    } catch (...) {
        std::cerr << "Failed to parse warpfile argument!:" << std::endl;
        return -1;
    }

    try {
        *manual = (bool) parser.has("M");
    } catch (...) {
        std::cerr << "Failed to parse manual argument." << std::endl;
        return -1;
    }

    try {
        *epsilon = parser.get<float>("e");
    } catch (...) {
        std::cerr << "Failed to parse epsilon argument!:" << std::endl;
        return -1;
    }

    try {
        std::map<std::string, int> motionTypeMap = {
            { "translation", cv::MOTION_TRANSLATION },
            { "euclidean"  , cv::MOTION_EUCLIDEAN   },
            { "affine"     , cv::MOTION_AFFINE      },
            { "homography" , cv::MOTION_HOMOGRAPHY  }
        };
        std::string motionTypeString = parser.get<std::string>("m");
        *motionType = motionTypeMap[motionTypeString];
        // assertion to prevent them from typing wrong and getting translation
        assert(motionTypeString != "translation" ? *motionType != 0 : true);
    } catch (...) {
        std::cerr << "Failed to parse motion_type argument!:" << std::endl;
        return -1;
    }

    try {
        *outputWarp = (std::string) parser.get<std::string>("o").c_str();
    } catch (...) {
        std::cerr << "Failed to parse output_warp argument!:" << std::endl;
        return -1;
    }

    try {
        *warpImgFilename = (std::string) parser.get<std::string>("w").c_str();
    } catch (...) {
        std::cerr << "Failed to parse warp_img_filename argument!:" << std::endl;
        return -1;
    }

    std::cout << std::endl << "Shortcuts:" << std::endl << "\tq\t- quit" << std::endl << std::endl;

    return 1;
}
