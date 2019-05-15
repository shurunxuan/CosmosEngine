set(PROJECT_NAME TestGame)

if (DEFINED ENV{VCPKG_ROOT})
	message("Found vcpkg, using vcpkg toolchain file")
	set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
endif()

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ..)

target_link_libraries(${PROJECT_NAME} PRIVATE CosmosEngine)