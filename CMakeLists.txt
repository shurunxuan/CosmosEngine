cmake_minimum_required(VERSION 3.14)

project(GameEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_THIRD_PARTY_SOURCE_DIRECTORY ${CMAKE_BINARY_DIR})

add_compile_definitions(__SOLUTION_DIR__="${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_compile_definitions(BOOST_ALL_NO_LIB)
    add_compile_definitions(BOOST_ALL_DYN_LINK)
else ()
    add_compile_definitions(BOOST_LOG_DYN_LINK)
endif ()

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

# Get ShaderConductor
find_package(Python3 COMPONENTS Interpreter REQUIRED)

set(SHADER_CONDUCTOR_DIR_FOUND FALSE)
set(SHADER_CONDUCTOR_CLI_EXECUTABLE_FOUND FALSE)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(SHADER_CONDUCTOR_CLI_EXECUTABLE_NAME ShaderConductorCmd.exe)
else ()
    set(SHADER_CONDUCTOR_CLI_EXECUTABLE_NAME ShaderConductorCmd)
endif ()

set(SHADER_CONDUCTOR_DIR_HINTS
        $ENV{PATH}
        ${CMAKE_THIRD_PARTY_SOURCE_DIRECTORY}/*/Bin/${SHADER_CONDUCTOR_CLI_EXECUTABLE_NAME}
        ${CMAKE_THIRD_PARTY_SOURCE_DIRECTORY}/ShaderConductor/Build/*/Bin/${SHADER_CONDUCTOR_CLI_EXECUTABLE_NAME})

if (NOT "$ENV{SHADERCONDUCTOR_ROOT}" STREQUAL "")
    set(SHADER_CONDUCTOR_DIR_HINTS
            ${SHADER_CONDUCTOR_DIR_HINTS}
            $ENV{SHADERCONDUCTOR_ROOT}/Bin)
endif ()

file(GLOB_RECURSE SHADER_CONDUCTOR_CLI_EXECUTABLE
        ${SHADER_CONDUCTOR_DIR_HINTS})
if (EXISTS ${SHADER_CONDUCTOR_CLI_EXECUTABLE})
    set(SHADER_CONDUCTOR_CLI_EXECUTABLE_FOUND TRUE)
    message("-- ShaderConductor Executable Found: ${SHADER_CONDUCTOR_CLI_EXECUTABLE}")
else ()
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set(SHADER_CONDUCTOR_BUILD_SYSTEM vs2017)
        set(SHADER_CONDUCTOR_COMPILER vc141)
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        set(SHADER_CONDUCTOR_BUILD_SYSTEM ninja)
        set(SHADER_CONDUCTOR_COMPILER clang)
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set(SHADER_CONDUCTOR_BUILD_SYSTEM ninja)
        set(SHADER_CONDUCTOR_COMPILER gcc)
    endif ()

    message("-- Compiling Shader Conductor from source")

    execute_process(COMMAND git clone https://github.com/microsoft/ShaderConductor.git ${CMAKE_THIRD_PARTY_SOURCE_DIRECTORY}/ShaderConductor)

    execute_process(COMMAND ${Python3_EXECUTABLE} ${CMAKE_THIRD_PARTY_SOURCE_DIRECTORY}/ShaderConductor/BuildAll.py ${SHADER_CONDUCTOR_BUILD_SYSTEM} ${SHADER_CONDUCTOR_COMPILER} x64 Release
            WORKING_DIRECTORY ${CMAKE_THIRD_PARTY_SOURCE_DIRECTORY}/ShaderConductor)

    file(GLOB_RECURSE SHADER_CONDUCTOR_CLI_EXECUTABLE
            ${CMAKE_THIRD_PARTY_SOURCE_DIRECTORY}/*/Bin/${SHADER_CONDUCTOR_CLI_EXECUTABLE_NAME}
            ${CMAKE_THIRD_PARTY_SOURCE_DIRECTORY}/ShaderConductor/Build/*/Bin/${SHADER_CONDUCTOR_CLI_EXECUTABLE_NAME})
    if (EXISTS ${SHADER_CONDUCTOR_CLI_EXECUTABLE})
        message("-- ShaderConductor Executable Found: ${SHADER_CONDUCTOR_CLI_EXECUTABLE}")
    else ()
        message(FATAL_ERROR "-- Can't find ShaderConductor Executable")
    endif ()

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
        CMAKE_CACHE_ARGS -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true
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



