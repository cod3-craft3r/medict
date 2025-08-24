#include "CommandHandler.hpp"

CommandHandler::CommandHandler(Database& database, const int argc, const char* argv[])
    : db(database)
{
    if (argc < 3) {
        std::cout << "wrong usage." << std::endl;
        exit(1);
    }
}