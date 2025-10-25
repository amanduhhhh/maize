cmake_minimum_required(VERSION 3.16)
project(Oubliette)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/SFML-2.6.1/include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

# Copy DLLs to output directory
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/SFML-2.6.1/bin/sfml-graphics-2.dll"
        "${CMAKE_SOURCE_DIR}/SFML-2.6.1/bin/sfml-window-2.dll"
        "${CMAKE_SOURCE_DIR}/SFML-2.6.1/bin/sfml-system-2.dll"
        "${CMAKE_SOURCE_DIR}/SFML-2.6.1/bin/openal32.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()
