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

std::string get_lower(std::string &str)
{
    for (char& c : str) {
        c = std::tolower(c);
    }

    return str;
}

std::filesystem::path getDataDir() {
    const char *xdgDataDir = std::getenv("XDG_DATA_HOME");
    std::string dataDir;

    if (xdgDataDir) {
        dataDir = std::string(xdgDataDir) + "/medict";
    }
    else {
        const char *homeDir = std::getenv("HOME");
        if (!homeDir) {
            throw std::runtime_error("$HOME is not set.");
        }
        dataDir = std::string(homeDir) + "/.local/share/medict";
    }

    if (!std::filesystem::exists(dataDir)) {
        try {
            std::filesystem::create_directories(dataDir);
            std::cout << "Created directories " << dataDir << std::endl;
        } catch (const std::filesystem::filesystem_error& e) {
            throw std::runtime_error("Couldn't create data directories: "+ std::string(e.what()));
        }
    }

    std::filesystem::path dbDir{dataDir};
    return dbDir;
}

std::string tm_to_str(const std::tm& t, const char* format /* = "%Y-%m-%d %H:%M:%S"*/) {
    std::stringstream ss;
    ss << std::put_time(&t, format);

    return ss.str();
}

std::tm str_to_tm(const std::string& str, const char* format /* = "%Y-%m-%d %H:%M:%S"*/) {
    std::tm t = {};
    std::stringstream ss(str);
    try {
        ss >> std::get_time(&t, format);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse time string" + std::string(e.what()));
    }

    // another check for failures
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse time string");
    }
    
    return t;
}