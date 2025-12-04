#include "SpacedRepetition.hpp"
#include <iomanip>
#include <sstream>

ReviewResult SpacedRepetition::calculateReview(
    int quality,
    double currentEF,
    int currentInterval,
    int currentReps,
    int correctCount,
    int totalReviews
) {
    ReviewResult result;
    result.quality = quality;

    // SM-2 Formula: EF' = EF + (0.1 - (5 - q) * (0.08 + (5 - q) * 0.02))
    double newEF = currentEF + (0.1 - (5.0 - quality) * (0.08 + (5.0 - quality) * 0.02));
    
    // Keep EF within bounds [1.3, 2.5]
    if (newEF < 1.3) newEF = 1.3;
    if (newEF > 2.5) newEF = 2.5;
    result.newEF = newEF;

    // Calculate next interval
    int nextInterval;
    if (quality < 3) {
        // Forgot it, restart from interval 0
        nextInterval = 0;
        result.newRepetitions = 0;
    } else {
        // Got it right (quality >= 3)
        if (currentReps == 0) {
            nextInterval = 1;  // First review: 1 day
        } else if (currentReps == 1) {
            nextInterval = 3;  // Second review: 3 days
        } else {
            // Subsequent reviews: multiply by EF
            nextInterval = static_cast<int>(std::round(currentInterval * newEF));
        }
        result.newRepetitions = currentReps + 1;
    }
    result.nextInterval = nextInterval;

    // Update correct ratio
    int newCorrectCount = (quality >= 3) ? correctCount + 1 : correctCount;
    int newTotalReviews = totalReviews + 1;
    result.newCorrectRatio = static_cast<double>(newCorrectCount) / newTotalReviews;

    return result;
}

std::string SpacedRepetition::getCurrentDateISO() {
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&nowTime);
    
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d");
    return oss.str();
}

std::string SpacedRepetition::addDaysToDate(const std::string& dateISO, int days) {
    // Parse ISO date (YYYY-MM-DD)
    std::tm t = {};
    std::istringstream ss(dateISO);
    ss >> std::get_time(&t, "%Y-%m-%d");
    
    if (ss.fail()) {
        return getCurrentDateISO();  // Return today if parsing fails
    }

    // Convert to time_t, add days, convert back
    std::time_t time = std::mktime(&t);
    time += days * 24 * 60 * 60;  // Add days in seconds
    
    std::tm* newTime = std::localtime(&time);
    
    std::ostringstream oss;
    oss << std::put_time(newTime, "%Y-%m-%d");
    return oss.str();
}

bool SpacedRepetition::isTermDueForReview(const std::string& nextReviewDate) {
    std::string today = getCurrentDateISO();
    // Simple string comparison works for YYYY-MM-DD format
    return nextReviewDate <= today;
}
