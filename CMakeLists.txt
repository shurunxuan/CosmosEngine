cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME CosmosEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(${PROJECT_NAME})

find_package(vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(${PROJECT_NAME} main.cpp Compiler.cpp)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw glm)

