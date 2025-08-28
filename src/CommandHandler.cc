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
            print_time_info(foundTerm->lastReviewDate, foundTerm->creationDate);
            std::cout << "Notes: " << foundTerm->notes << "\n";
            delete foundTerm; // Free the allocated memory
        }
        // Display other details as needed
    } else {
        std::cout << "Nothing found for: " << name << "\n";
    }
}