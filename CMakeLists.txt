cmake_minimum_required(VERSION 3.16)
project(Oubliette)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} SFML::Graphics SFML::Window SFML::System)

# macOS doesn't need DLL copying - SFML is linked statically
