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
    std::string* imageFilename,
    std::string* templateFilename,
    std::string* warpFilename,
    bool* manual,
    float* epsilon
) {
    cv::String keys =
        "{@imagefile    |<none>| Input Image}"
        "{@templatefile |<none>| Template Image}"
        "{@warpfile     |<none>| Warp Matrix}"
        "{manual m      |      | Perform Manual Registration}"
        "{epsilon e     |      | ECC's Convergence Epsilon}"
        "{help h        |      | Show Help Message}";

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
        *manual = (bool) parser.has("m");
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

    return 1;
}
