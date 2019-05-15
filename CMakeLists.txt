cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (DEFINED ENV{VCPKG_ROOT})
	message("Found vcpkg, using vcpkg toolchain file")
	set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
#	set(CMAKE_MACOSX_RPATH 1)
	set(MACOS_CREATE_BUNDLE FALSE)
endif()

add_subdirectory(CosmosEngine)
add_subdirectory(TestGame)



