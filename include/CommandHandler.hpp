#include <string>
#include "Database.hpp"

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
    void list_all_terms();
    void handleAddCommand(const std::string& name, const std::string& def, const std::string& cat);
    void handleSearchCommand(const std::string& name);
    void handleLearnedStatusCommand(const std::string& name, bool status);
};