cmake_minimum_required(VERSION 3.14)

project(GameEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_definitions(__SOLUTION_DIR__="${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
add_compile_definitions(BOOST_ALL_NO_LIB)
add_compile_definitions(BOOST_ALL_DYN_LINK)

# Find glslangValidator
find_package(Vulkan REQUIRED)

get_filename_component(VULKAN_LIBRARY_PATH ${Vulkan_LIBRARY} DIRECTORY)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(GLSL_VALIDATOR "${VULKAN_LIBRARY_PATH}/../Bin/glslangValidator.exe")
    else ()
        set(GLSL_VALIDATOR "${VULKAN_LIBRARY_PATH}/../Bin32/glslangValidator.exe")
    endif ()
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(GLSL_VALIDATOR "${VULKAN_LIBRARY_PATH}/../bin/glslangValidator")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    find_file(GLSL_VALIDATOR
            glslangValidator
            HINTS ${VULKAN_LIBRARY_PATH}/../../)
endif ()

get_filename_component(GLSL_VALIDATOR ${GLSL_VALIDATOR} REALPATH)

if (EXISTS ${GLSL_VALIDATOR})
    message("-- Found GLSL Validator: ${GLSL_VALIDATOR}")
else ()
    message(SEND_ERROR "GLSL Validator Not Found!")
endif ()

# Operating System Specification
if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    #	set(CMAKE_MACOSX_RPATH 1)
    set(MACOS_CREATE_BUNDLE FALSE)
endif ()

# SPRIV-Cross
include(ExternalProject)
find_package(Git REQUIRED)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

ExternalProject_Add(
        SPIRV-Cross
        GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git
        UPDATE_COMMAND ${GIT_EXECUTABLE} pull
        LOG_DOWNLOAD ON
        CMAKE_ARGS -DSPIRV_CROSS_SHARED=OFF -DSPIRV_CROSS_CLI=OFF -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)

set(
        SPRIV_CROSS_LIBRARIES
        spirv-cross-c
        spirv-cross-core
        spirv-cross-cpp
        spirv-cross-glsl
        spirv-cross-hlsl
        spirv-cross-msl
        spirv-cross-reflect
        spirv-cross-util
)

ExternalProject_Add(
        SPIRV-Headers
        GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Headers.git
        UPDATE_COMMAND ${GIT_EXECUTABLE} pull
        LOG_DOWNLOAD ON
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)

# Engine and Game
add_subdirectory(CosmosEngine)
add_subdirectory(TestGame)



