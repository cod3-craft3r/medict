#include "util.hpp"
#include "CommandHandler.hpp"

void CommandHandler::set_database(std::unique_ptr<Database> database) {
    db = std::move(database);
    std::cout << "Database has been set in CommandHandler.\n";
}

void CommandHandler::list_all_terms()
{
    db->list_all_terms();
}