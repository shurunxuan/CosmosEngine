set(PROJECT_NAME CosmosEngine)
set(ENGINE_NAME ${PROJECT_NAME})

if (DEFINED ENV{VCPKG_ROOT})
	message("Found vcpkg, using vcpkg toolchain file")
	set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
endif()

project(${PROJECT_NAME})

find_package(vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_compile_definitions(ENGINE_EXPORTS)

add_library(${PROJECT_NAME} SHARED App/App.cpp Compiler/Compiler.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw glm)