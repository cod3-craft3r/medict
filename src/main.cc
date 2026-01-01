// #include "Application.hpp"
// #include "Render.hpp"
// #include <thread>

bool gVERBOSE = false;
void setVerbose() {
    gVERBOSE = true;
}

// int main(const int argc, const char* argv[]) {
    // std::cout << "Welcome to <APPNAME> v0.1.\nMade with 💜." << std::endl;
    // std::cout << "If this is your first time using this app, a good idea is to type in 'help' in the prompt below." << std::endl;

    // for (int i=0; i < argc; ++i) {
    //     std::string arg = argv[i];
    //     if (arg == "--verbose" || arg == "-v") {
    //         setVerbose();
    //         std::cout << "Verbose mode enabled." << std::endl;
    //     }
    // }

//     std::unique_ptr<Application> app = std::make_unique<Application>();
    
//     // Launch Application Logic in a Worker Thread
//     std::thread appThread([&]() {
//         app->run();
//     });

//     // Run SDL/UI Logic on the Main Thread
//     runRenderLoop(Application::running);

//     // Join when done
//     if (appThread.joinable()) {
//         appThread.join();
//     }

//     return 0;
// }

#include "Application.hpp"
#include "Render.hpp"
#include <thread>

// ... (verbose logic) ...

int main(const int argc, const char* argv[]) {
    std::cout << "Welcome to <APPNAME> v0.1.\nMade with 💜." << std::endl;
    std::cout << "If this is your first time using this app, a good idea is to type in 'help' in the prompt below." << std::endl;

    for (int i=0; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--verbose" || arg == "-v") {
            setVerbose();
            std::cout << "Verbose mode enabled." << std::endl;
        }
    }

    std::unique_ptr<Application> app = std::make_unique<Application>();
    
    // Launch Application Logic in a Worker Thread
    std::thread appThread([&]() {
        app->run();
    });

    // Run SDL/UI Logic on the Main Thread
    runRenderLoop(Application::running);

    // Join when done
    if (appThread.joinable()) {
        appThread.join();
    }

    return 0;
}
