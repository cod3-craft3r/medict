#include "CommandHandler.hpp"

void CommandHandler::set_database(std::unique_ptr<Database> database) {
    db = std::move(database);
    std::cout << "Database has been set in CommandHandler.\n";
}

void CommandHandler::list_terms_in_know_base()
{
    db->list_all_terms();
}

bool CommandHandler::add_term_to_know_base()
{
    std::string name;
    std::string cat;
    std::string def;
    std::string notes;
    // bool cancelled = false;

    std::cout << "Enter term (or type 'q' to stop adding): ";
    std::getline(std::cin, name);
    if (get_lower(name) == "q") {
        return false;
    }

    std::cout << "Enter category (or type 'exit' to stop adding): ";
    std::getline(std::cin, cat);
    if (get_lower(cat) == "q") {
        return false;
    }
    
    std::cout << "Enter definition (or type 'exit' to stop adding): ";
    std::getline(std::cin, def);
    if (get_lower(def) == "q") {
        return false;
    }
    
    std::cout << "Enter notes (or type 'exit' to stop adding): ";
    std::getline(std::cin, notes);
    if (get_lower(notes) == "q") {
        return false;
    }

    Term newTerm(name, def, cat, notes);

    // TODO))
    // if (!db->searchTerm(newTerm)) // only add if term doesn't already exist
    return db->add_term(newTerm);
}

void CommandHandler::find_term()
{
    std::string name;
    std::cout << "which term to search for: ";
    std::getline(std::cin, name);
    std::vector<Term *> foundTerms = db->search_term((const std::string) name);
    if (foundTerms.size() > 0) {
        std::cout << "Terms found: " << foundTerms.size() << "\n";
        for(auto& foundTerm : foundTerms) {
            std::cout << "--------------------\n";
            std::cout << "Term: " << foundTerm->termName << "\n";
            std::cout << "Definition: " << foundTerm->definition << "\n";
            std::cout << "Category: " << foundTerm->category << "\n";
            std::cout << "Learned Status: " << (foundTerm->learnedStatus ? "Learned" : "Not Learned") << "\n";
            std::cout << "Created on: " << foundTerm->creationDate << "\n";
            std::cout << "Last reviewed: " << foundTerm->lastReviewDate << "\n";
            std::cout << "Notes: " << foundTerm->notes << "\n";

            // std::cout << "Do you want to load the image associated w/ this term? (yes/no): ";
            // std::string response;
            // std::getline(std::cin, response);
            // if (get_lower(response) == "yes") {
            //     nlohmann::json imagesToLoad = 
            // }

            delete foundTerm; // Free the allocated memory
        }
        // Display other details as needed
    } else {
        std::cout << "Nothing found for: " << name << "\n";
    }
}

void CommandHandler::showDueTerms()
{
    std::cout << "\n=== Terms Due for Review ===\n";
    std::vector<Term *> dueTerms;
    
    // Get all terms and filter due ones
    std::vector<Term *> allTerms = db->search_term("");  // Empty search returns all
    
    for (auto& term : allTerms) {
        if (SpacedRepetition::isTermDueForReview(term->nextReviewDate)) {
            dueTerms.push_back(term);
        } else {
            delete term;
        }
    }
    
    if (dueTerms.empty()) {
        std::cout << "No terms due for review today!\n";
        return;
    }
    
    std::cout << "You have " << dueTerms.size() << " term(s) to review:\n\n";
    for (size_t i = 0; i < dueTerms.size(); ++i) {
        std::cout << (i + 1) << ". " << dueTerms[i]->termName 
                  << " (Category: " << dueTerms[i]->category << ")\n";
    }
    std::cout << "\n";
    
    // Clean up
    for (auto& term : dueTerms) {
        delete term;
    }
}

void CommandHandler::reviewTerm()
{
    std::cout << "\n=== Review Mode ===\n";
    
    // Get all terms
    std::vector<Term *> allTerms = db->search_term("");

    // i could make the fn above return a list of all the terms to be reviewed...
    std::vector<Term *> dueTerms;
    for (auto& term : allTerms) {
        if (SpacedRepetition::isTermDueForReview(term->nextReviewDate)) {
            dueTerms.push_back(term);
        } else {
            delete term;
        }
    }
    
    if (dueTerms.empty()) {
        std::cout << "No terms due for review!\n";
        return;
    }
    
    std::cout << "Starting review session with " << dueTerms.size() << " term(s)...\n";
    
    for (size_t i = 0; i < dueTerms.size(); ++i) {
        Term* term = dueTerms[i];
        std::cout << "=== Question " << (i + 1) << "/" << dueTerms.size() << " ===\n";
        std::cout << "Term: " << term->termName << "\n";
        std::cout << "Category: " << term->category << "\n";
        std::cout << "\nTry to recall the definition before pressing Enter...\n";
        std::cin.get();
        
        std::cout << "\nDefinition: " << term->definition << "\n";
        std::cout << "Notes: " << term->notes << "\n";
        
        std::cout << "\nHow well did you remember? (0-5)\n";
        std::cout << "  0: Completely forgot\n";
        std::cout << "  1: Very difficult\n";
        std::cout << "  2: Difficult\n";
        std::cout << "  3: Difficult but got it\n";
        std::cout << "  4: Good\n";
        std::cout << "  5: Perfect!\n";
        std::cout << "Enter (0-5): ";
        
        int quality;
        std::cin >> quality;
        std::cin.ignore();  // Clear the newline from buffer
        
        if (quality < 0 || quality > 5) {
            std::cout << "Invalid input. Skipping...\n";
            continue;
        }
        
        updateTermReview(term->termName, quality);
        std::cout << "\n";
    }
    
    // Clean up
    for (auto& term : dueTerms) {
        delete term;
    }
    
    std::cout << "Review session complete!\n";
}

void CommandHandler::updateTermReview(const std::string& termName, int quality)
{
    // Create a copy to make lowercase for search
    std::string lowerTermName = termName;
    for (char& c : lowerTermName) {
        c = std::tolower(c);
    }
    std::vector<Term *> foundTerms = db->search_term(lowerTermName);
    
    if (foundTerms.empty()) {
        std::cout << "Failed to update after review. Term not found.\n";
        return;
    }
    
    Term* term = foundTerms[0];  // i need to make my searching robust. really robust.
    
    // Calculate new SR values
    ReviewResult result = SpacedRepetition::calculateReview(
        quality,
        term->easinessFactor,
        term->interval,
        term->repetitionCount,
        static_cast<int>(term->correctRatio * term->reviewCount),  // correct count
        term->reviewCount
    );
    
    // Update term
    term->easinessFactor = result.newEF;
    term->interval = result.nextInterval;
    term->repetitionCount = result.newRepetitions;
    term->lastReviewDate = SpacedRepetition::getCurrentDateISO();
    term->nextReviewDate = SpacedRepetition::addDaysToDate(term->lastReviewDate, result.nextInterval);
    term->reviewCount++;
    term->correctRatio = result.newCorrectRatio;
    
    // Save updated term to database
    db->updateTerm(*term);
    
    // Display feedback
    if (quality >= 3) {
        std::cout << "✓ Good! Next review in " << result.nextInterval << " day(s).\n";
    } else {
        std::cout << "✗ Let's review this again soon. Next review in " << result.nextInterval << " day(s).\n";
    }
    
    // Clean up
    for (auto& t : foundTerms) {
        delete t;
    }
}
