cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(assignment2)

include(cmake/get_cpm.cmake)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

# Dependencies
CPMAddPackage("gh:raysan5/raylib#5.0")

# Static library of utilities
add_library(gg_lib STATIC

    src/gg/grid.c
    src/gg/astar.c
    src/gg/camera.c

    )
target_link_libraries(gg_lib raylib)

# Our Project
add_executable(${PROJECT_NAME} 

    src/main.c 
    
    )

target_link_libraries(${PROJECT_NAME} gg_lib)

# Assets
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/Debug/assets
)
add_dependencies(${PROJECT_NAME} copy_assets)
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_compile_definitions(${PROJECT_NAME} PUBLIC PROJECT_NAME=${PROJECT_NAME})

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()