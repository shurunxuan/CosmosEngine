set(PROJECT_NAME CosmosEngine)
set(ENGINE_NAME ${PROJECT_NAME})
project(${PROJECT_NAME})

find_package(vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_compile_definitions(ENGINE_EXPORTS)

add_library(${PROJECT_NAME} SHARED App/App.cpp Compiler/Compiler.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw glm)