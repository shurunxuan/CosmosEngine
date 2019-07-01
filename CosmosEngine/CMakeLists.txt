set(PROJECT_NAME CosmosEngine)
set(ENGINE_NAME ${PROJECT_NAME})

project(${PROJECT_NAME})

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_compile_definitions(ENGINE_EXPORTS)

set(SOURCE_FILES
        App/App.cpp
        Compiler/Compiler.cpp
        )

set(HEADER_FILES
        App/App.h
        Compiler/Compiler.h
        )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan glfw glm)