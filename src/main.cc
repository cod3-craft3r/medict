#include "Application.hpp"

int main(const int argc, const char* argv[]) {
    std::cout << "Welcome to <APPNAME> v0.1.\nMade with 💜." << std::endl;
    std::cout << "If this is your first time using this app, a good idea is to type in 'help' in the prompt below." << std::endl;

    std::unique_ptr<Application> app = std::make_unique<Application>();
    app->run();

    return 0;
}