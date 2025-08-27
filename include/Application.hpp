#pragma once
#include <memory>
#include <csignal>
#include <atomic>
#include <iostream>
#include "CommandHandler.hpp"

class Application {
private:
    static std::atomic<bool> running;
    // CommandHandler cmdHandler;
    std::unique_ptr<Database> db;
    std::unique_ptr<CommandHandler> cmdHandler;

    static void signalHandler(int signum);
    void initSignalHandlers();
    void validateArgs(const int argc, const char* argv[]);
    
public:
    Application();
    ~Application() = default;
    
    // Prevent copying
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    
    // Allow moving
    Application(Application&&) = default;
    Application& operator=(Application&&) = default;

    void run();
    void shutdown();
    static bool isRunning() { return running; }
};