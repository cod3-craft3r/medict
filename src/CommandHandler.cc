#include "util.hpp"
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