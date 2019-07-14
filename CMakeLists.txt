cmake_minimum_required(VERSION 3.14)

project(GameEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_definitions(__SOLUTION_DIR__="${CMAKE_CURRENT_SOURCE_DIR}")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    #	set(CMAKE_MACOSX_RPATH 1)
    set(MACOS_CREATE_BUNDLE FALSE)
endif ()

add_subdirectory(CosmosEngine)
add_subdirectory(TestGame)



