#include "Application.hpp"

std::atomic<bool> Application::running{true};

Application::Application() {
    initSignalHandlers();
    // validateArgs(argc, argv); we don't even need this. User enters the application just once & then we decide along with the user as to which knowledge space they wish to load/delete/modify.
    // init();
    db = std::make_unique<Database>();
    cmdHandler = std::make_unique<CommandHandler>();
}

void Application::signalHandler(int signum) {
    if (signum == SIGINT) {
        std::cout << "\nReceived SIGINT. Shutting down gracefully... :-)\n";
        running = false;
    }
}

void Application::initSignalHandlers() {
    std::signal(SIGINT, Application::signalHandler);
}

// void Application::init() {
//     // checking the db file's path & loading it should be good enough for now.
//     // later, maybe some dynamic mem allocation could also be done, if required.
// }

void Application::run() {
    //first we load the data.
    db->loadData();
    cmdHandler->set_database(std::move(db));

    std::string command;
    while (running) {
        bool validCommand = false;
        std::cout << "-> ";
        std::getline(std::cin, command);
        
        if (command == "exit" || command == "quit") {
            shutdown();
            break;
        }

        // then we check the actual command...
        else if (get_lower(command) == "add") {
            std::cout << "calling function to add" << "\n";
            validCommand = true;
        }
        else if (get_lower(command) == "ls") {
            cmdHandler->list_all_terms();
            validCommand = true;
        }
        else if (get_lower(command) == "mod") {
            std::cout << "calling function to mod" << "\n";
            validCommand = true;
        }
        else if (get_lower(command) == "del") {
            std::cout << "calling function to del" << "\n";
            validCommand = true;
        }
        else if (get_lower(command) == "find") {
            std::cout << "calling function to find" << "\n";
            validCommand = true;
        }
        else if (get_lower(command) == "help") {
            std::cout << "Available commands: -\n";
            std::cout << "  - add:           Add a new Term to the Knowledge Base\n";
            std::cout << "  - ls:            List all Terms in the Knowledge Base\n";
            std::cout << "  - mod:           Modify an existing Term in the Knowledge Base\n";
            std::cout << "  - del:           Delete a Term from the Knowledge Base\n";
            std::cout << "  - find:          Search for a Term in the Knowledge Base\n";
            std::cout << "  - help:          Show this help message\n";
            std::cout << "  - exit or quit:  Exit the application\n";
            // validCommand = true;
        }
        else {
            std::cout << command << " is not a valid command. Try again.\n";
        }

        // if (validCommand)
        //     cmdHandler.handleCommand(command);
    }
}

void Application::shutdown() {
    running = false;
    // Add cleanup code here
    std::cout << "Shutting down...\n";
}

void Application::validateArgs(const int argc, const char* argv[]) {
    // Initialize your data/db here
    if (argc < 3) {
        std::cout << "wrong usage." << std::endl;
        std::cout << "Usage: medict < COMMAND > < TERM >" << std::endl;
        exit(1);
    }
    // it would only make sense to parse the path of the db file at the very beginning. and just check if it even exists.
    // if it doesn't exist, we ask to either create a new one or ask for a valid path.
    // if it does exist, we load the data from it...but i'm a little worried about the performance of loading a large json file, especially about which data structure to use.

    // i expect the user to not even know about the location where the db files are stored...
    // i mean it only increases the cognitive load on them...
}