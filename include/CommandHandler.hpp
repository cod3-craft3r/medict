#include <string>
#include "Database.hpp"
#include "util.hpp"
#include "Term.hpp"

class CommandHandler {
private:
    // db ptr?
    std::unique_ptr<Database> db;

public:
    // Constructor
    CommandHandler() = default;
    ~CommandHandler() = default;
    CommandHandler(const CommandHandler&) = delete;  // Prevent copying
    CommandHandler& operator=(const CommandHandler&) = delete;
    CommandHandler(CommandHandler&&) noexcept = default;  // Allow moving
    CommandHandler& operator=(CommandHandler&&) noexcept = default;

    void set_database(std::unique_ptr<Database> database);
    // Methods to handle commands
    void list_terms_in_know_base();
    bool add_term_to_know_base();
    void find_term();
    void handleLearnedStatusCommand(const std::string& name, bool status);
};