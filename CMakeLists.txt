cmake_minimum_required(VERSION 3.10)
project(SpaceInvaders)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add executable
add_executable(SpaceInvaders main.cpp)

# Link SFML libraries
target_link_libraries(SpaceInvaders sfml-graphics sfml-window sfml-system)

# Include directories
target_include_directories(SpaceInvaders PRIVATE ${SFML_INCLUDE_DIR})
