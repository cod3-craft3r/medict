#include "Application.hpp"

int main(const int argc, const char* argv[]) {
    std::cout << "Heloo!" << std::endl;
    // Term t("E1", "this is an example term", "example");

    std::unique_ptr<Application> app = std::make_unique<Application>();
    app->run();

    return 0;
}