set(PROJECT_NAME CosmosEngine)
set(ENGINE_NAME ${PROJECT_NAME})

project(${PROJECT_NAME})

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_compile_definitions(ENGINE_EXPORTS)

add_library(${PROJECT_NAME} SHARED App/App.h App/App.cpp)
add_library(${PROJECT_NAME} SHARED Compiler/Compiler.h Compiler/Compiler.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan glfw glm)