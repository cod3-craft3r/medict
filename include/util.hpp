#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <sstream>

void print_time_info(std::tm lastReview, std::tm creation);
std::string get_lower(std::string &str);
std::filesystem::path getDataDir();
std::string tm_to_str(const std::tm& t, const char* format = "%Y-%m-%d %H:%M:%S");
std::tm str_to_tm(const std::string& str, const char* format = "%Y-%m-%d %H:%M:%S");