cmake_minimum_required(VERSION 3.10)
project(medict)

add_executable(medict 
    src/main.cc
    src/Application.cc
    src/CommandHandler.cc
    src/Database.cc
    src/util.cc
)

target_include_directories(medict PRIVATE include/)
