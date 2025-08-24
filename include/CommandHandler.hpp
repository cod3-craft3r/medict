#include <string>
#include "Database.hpp"

class CommandHandler {
private:
    Database& db; // Reference to the Database

public:
    // Constructor
    CommandHandler(Database& database, const int argc, const char* argv[]);

    // Methods to handle commands
    void handleAddCommand(const std::string& name, const std::string& def, const std::string& cat);
    void handleSearchCommand(const std::string& name);
    void handleListCommand();
    void handleLearnedStatusCommand(const std::string& name, bool status);
};