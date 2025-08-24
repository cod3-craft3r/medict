#include <chrono>
#include <ctime>
#include <string>
#include <iostream>
#include <typeinfo>

class Term {
public:
    std::string  termName;
    std::string  definition;
    std::string  category;
    bool         learnedStatus;
    std::tm      creationDate;
    std::tm      lastReviewDate;
    std::string  notes;

    // Constructor, getter and setter functions.
    Term(std::string name, std::string def, std::string cat)
        : termName(name), definition(def), category(cat), learnedStatus(false)
        {
            // lastReviewDate = "empty";
            // notes = "empty";
            // Get current time
            auto now = std::chrono::system_clock::now();

            // Convert to time_t to make it compatible with std::localtime
            std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
            
            // Convert to local time
            this->creationDate = *std::localtime(&nowTime);
            this->lastReviewDate = *std::localtime(&nowTime);
            
            // Display the time in a nice format (e.g., YYYY-MM-DD HH:MM:SS)
            // std::cout << "Current time: "
            //   << std::put_time(&localTime, "%a, %Y-%m-%d %H:%M:%S") << std::endl;
        }
};
