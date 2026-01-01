#pragma once

#include "util.hpp"

// SM-2 Spaced Repetition Algorithm implementation

struct ReviewResult {
    int quality;        // 0-5 scale (0-2 = forgot, 3-4 = struggled, 5 = perfect)
    int nextInterval;   // Days until next review
    double newEF;       // Updated easiness factor
    int newRepetitions; // Updated repetition count
    double newCorrectRatio;  // Updated correct answer ratio
};

class SpacedRepetition {
public:
    // Calculate review interval based on SM-2 algorithm
    // quality: 0-5 (user's rating of how well they remembered)
    // currentEF: current easiness factor
    // currentInterval: current interval in days
    // currentReps: current repetition count
    // correctCount: number of correct reviews so far
    // totalReviews: total number of reviews so far
    static ReviewResult calculateReview(
        int quality,
        double currentEF,
        int currentInterval,
        int currentReps,
        int correctCount,
        int totalReviews
    );

    // Get current date in ISO 8601 format
    static std::string getCurrentDateISO();

    // Add days to a date (ISO format: YYYY-MM-DD)
    static std::string addDaysToDate(const std::string& dateISO, int days);

    // Check if a term is due for review
    static bool isTermDueForReview(const std::string& nextReviewDate);
};
