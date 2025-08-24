#include <iostream>
#include <ctime>
#include <chrono>
#include "util.hpp"

// Utility functions can be added here
void print_time_info(std::tm lastReview, std::tm creation)
{
    auto now = std::chrono::system_clock::now();
    // Convert to time_t to make it compatible with std::localtime
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&nowTime);

    if (lastReview.tm_year == localTime.tm_year) {
        if (lastReview.tm_yday == localTime.tm_yday) {
            std::cout << "Created on: "
              << std::put_time(&creation, "%Y-%m-%d") << std::endl;
            std::cout << "This was last reviewed today." << std::endl;
        }
        else if (lastReview.tm_yday + 1 == localTime.tm_yday) {
            std::cout << "Created on: "
              << std::put_time(&creation, "%Y-%m-%d") << std::endl;
            std::cout << "This was last reviewed yesterday." << std::endl;
        }
        else if ((lastReview.tm_yday - localTime.tm_yday) < 7) {
            std::cout << "Created on: "
              << std::put_time(&creation, "%Y-%m-%d") << std::endl;
            std::cout << "Last reviewed on "
              << std::put_time(&lastReview, "%a") << std::endl;
        }
        else {
            std::cout << "Created on: "
              << std::put_time(&creation, "%Y-%m-%d") << std::endl;
            std::cout << "Last reviewed on: "
              << std::put_time(&lastReview, "%Y-%m-%d") << std::endl;
        }
    }
    // std::cout << "Current time: "
    //           << std::put_time(&localTime, "%a, %Y-%m-%d %H:%M:%S") << std::endl;
}