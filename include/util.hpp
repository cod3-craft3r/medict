#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <fstream>
#include <filesystem>

void print_time_info(std::tm lastReview, std::tm creation);
std::string get_lower(std::string &str);
std::filesystem::path getDataDir();