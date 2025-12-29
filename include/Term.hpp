#pragma once
#include <chrono>
#include <ctime>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

class Term {
public:
    std::string  termName;
    std::string  definition;
    std::string  category;
    bool         learnedStatus;
    std::string  creationDate;        // ISO 8601 format: YYYY-MM-DD
    std::string  lastReviewDate;      // ISO 8601 format: YYYY-MM-DD
    std::string  notes;
    
    // Spaced Repetition Algorithm fields (SM-2)
    double       easinessFactor;      // Starts at 2.5, range [1.3, 2.5]
    int          interval;            // Days until next review
    int          repetitionCount;     // Consecutive successful reviews
    std::string  nextReviewDate;      // ISO 8601 format: YYYY-MM-DD
    int          reviewCount;         // Total review attempts
    double       correctRatio;        // Correct answers / total reviews

    Term() = default;
    
    // Constructor for new terms
    Term(std::string name, std::string def, std::string cat, std::string notes)
        : termName(name), definition(def), category(cat), learnedStatus(false), notes(notes),
          easinessFactor(2.5), interval(0), repetitionCount(0), reviewCount(0), correctRatio(0.0)
        {
            std::string currentDate = getCurrentDateISO();
            this->creationDate = currentDate;
            this->lastReviewDate = currentDate;
            this->nextReviewDate = currentDate;  // Review immediately
        }

private:
    // Helper to get current date in ISO 8601 format (YYYY-MM-DD)
    static std::string getCurrentDateISO() {
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        std::tm* localTime = std::localtime(&nowTime);
        
        std::ostringstream oss;
        oss << std::put_time(localTime, "%Y-%m-%d");
        return oss.str();
    }
};
