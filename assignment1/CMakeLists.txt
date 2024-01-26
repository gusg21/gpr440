cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(gpr440_assignment1)

include(cmake/get_cpm.cmake)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Dependencies
CPMAddPackage("gh:raysan5/raylib#5.0")

# Our Project
add_executable(${PROJECT_NAME} src/generation.c src/main.c )
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_compile_definitions(${PROJECT_NAME} PUBLIC PROJECT_NAME=${PROJECT_NAME})

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()