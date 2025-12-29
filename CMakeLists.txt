cmake_minimum_required(VERSION 3.10)
project(medict-dev)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

set(CXX_FLAGS "-Wall -Wextra -Wpedantic -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")
message(STATUS "${CMAKE_CXX_FLAGS}")

file(GLOB SOURCES "src/*.cc")

add_executable(medict-dev 
    ${SOURCES}
)

target_include_directories(medict-dev PRIVATE include/)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

target_include_directories(medict-dev PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(medict-dev PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
