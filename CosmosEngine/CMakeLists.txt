set(PROJECT_NAME CosmosEngine)
set(ENGINE_NAME ${PROJECT_NAME})

project(${PROJECT_NAME})

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS log REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_compile_definitions(ENGINE_EXPORTS)

set(SOURCE_FILES
        App/App.cpp
        Rendering/RenderingBackend.cpp
        Rendering/Vulkan/SPRIV_Reflect/spirv_reflect.c
#        Rendering/ReflectionalShader.cpp
        Rendering/Vulkan/VulkanBackend.cpp
        Logging/Logging.cpp
#        Rendering/Vulkan/ReflectionalSpirV.cpp
        )

set(HEADER_FILES
        Export.h
        App/App.h
        Rendering/RenderingBackend.h
                Rendering/Vulkan/SPRIV_Reflect/spirv_reflect.h
#        Rendering/ReflectionalShader.h
        Rendering/Vulkan/VulkanBackend.h
        Logging/Logging.h
#        Rendering/Vulkan/ReflectionalSpirV.h
        )

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_ALL_DYN_LINK)

# Shaders (SPIR-V)

# Delete last compilation folder
file(REMOVE_RECURSE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders/)

# Get all glsl shaders
file(GLOB_RECURSE GLSL_SHADERS
        "Rendering/Shaders/*.frag"
        "Rendering/Shaders/*.vert"
        )


foreach (GLSL ${GLSL_SHADERS})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    file(RELATIVE_PATH REL_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/Shaders/ ${GLSL})
    set(SPIRV "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders/${REL_FILE_NAME}.spv")
    if (${REL_FILE_NAME} STREQUAL ${FILE_NAME})
        set(REL_PATH ".")
    else ()
        get_filename_component(REL_PATH ${REL_FILE_NAME} DIRECTORY)
    endif ()

    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders/${REL_PATH}/"
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach (GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(${PROJECT_NAME} Shaders SPIRV-Headers)

target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan glfw glm ${Boost_LIBRARIES} ${ASSIMP_LIBRARY})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} PUBLIC bcrypt)
endif ()