set(PROJECT_NAME CosmosEngine)
set(ENGINE_NAME ${PROJECT_NAME})

project(${PROJECT_NAME})


add_compile_definitions(ENGINE_EXPORTS)

set(SOURCE_FILES
        App/App.cpp
        Rendering/RenderingBackend.cpp
        Rendering/ReflectionalShader.cpp
        Rendering/Vulkan/VulkanBackend.cpp
        Logging/Logging.cpp
        Rendering/Vulkan/ReflectionalSpirV.cpp
        Core/Object.cpp
        Core/Component.cpp
        Core/Transform.cpp
        Core/Scene.cpp
        Core/Camera.cpp
        Rendering/RenderingPipeline.cpp
        Core/MeshRenderer.cpp
        Core/Mesh.cpp
        Core/Material.cpp
        Core/Texture.cpp
        Rendering/CommandBuffer.cpp
        Rendering/Vulkan/VulkanCommandBuffer.cpp
        Input/InputBackend.cpp
        Input/GLFW/GLFWInputBackend.cpp
        Physics/PhysicsSystem.cpp
        Core/Collider.cpp
        Core/BoxCollider.cpp
        Core/Rigidbody.cpp)

set(HEADER_FILES
        Export.h
        App/App.h
        Rendering/RenderingBackend.h
        Rendering/ReflectionalShader.h
        Rendering/Vulkan/VulkanBackend.h
        Logging/Logging.h
        Rendering/Vulkan/ReflectionalSpirV.h
        Core/Object.h
        Core/Component.h
        Core/Transform.h
        Core/Scene.h
        Core/Camera.h
        Rendering/RenderingPipeline.h
        Rendering/Vulkan/VulkanPipeline.h
        Rendering/Vulkan/VulkanPipeline.cpp
        Core/MeshRenderer.h
        Core/Mesh.h
        Core/Material.h
        Core/Texture.h
        Rendering/CommandBuffer.h
        Rendering/Vulkan/VulkanCommandBuffer.h
        Input/InputBackend.h
        Input/GLFW/GLFWInputBackend.h
        Physics/PhysicsSystem.h
        Core/Collider.h
        Core/BoxCollider.h
        Core/Rigidbody.h)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

# Shaders (SPIR-V)

# Delete last compilation folder
file(REMOVE_RECURSE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders/)

# Shaders
#file(GLOB_RECURSE SHADER_FILES
#        "Rendering/Shaders/*.frag"
#        "Rendering/Shaders/*.vert"
#        "Rendering/Shaders/*.hlsl"
#        )
set(SHADER_FILES
        Rendering/Shaders/shader.vert
        Rendering/Shaders/shader.frag
        Rendering/Shaders/VertexShader.hlsl
        Rendering/Shaders/PixelShader.hlsl
        )

set_source_files_properties(Rendering/Shaders/shader.vert
        PROPERTIES
        lang glsl
        stage vs)

set_source_files_properties(Rendering/Shaders/shader.frag
        PROPERTIES
        lang glsl
        stage ps)

set_source_files_properties(Rendering/Shaders/VertexShader.hlsl
        PROPERTIES
        lang hlsl
        stage vs)

set_source_files_properties(Rendering/Shaders/PixelShader.hlsl
        PROPERTIES
        lang hlsl
        stage ps)

foreach (SHADER_FILE ${SHADER_FILES})
    get_source_file_property(SHADER_LANG ${SHADER_FILE} lang)
    get_source_file_property(SHADER_STAGE ${SHADER_FILE} stage)

    get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
    file(RELATIVE_PATH REL_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/Rendering/Shaders/ ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_FILE})
    set(SPIRV "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders/${REL_FILE_NAME}.spv")
    if (${REL_FILE_NAME} STREQUAL ${FILE_NAME})
        set(REL_PATH ".")
    else ()
        get_filename_component(REL_PATH ${REL_FILE_NAME} DIRECTORY)
    endif ()

    set(SHADER_FILE_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders/${REL_PATH}/)
    set(INPUT_SHADER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_FILE})
    if (${SHADER_LANG} STREQUAL "glsl")
        add_custom_command(
                OUTPUT ${SPIRV}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_FILE_OUTPUT_DIR}
                COMMAND ${GLSL_VALIDATOR} -V ${INPUT_SHADER_FILE} -o ${SPIRV}
                DEPENDS ${SHADER_FILE})
        list(APPEND SPIRV_BINARY_FILES ${SPIRV})
    elseif (${SHADER_LANG} STREQUAL "hlsl")
        add_custom_command(
                OUTPUT ${SPIRV}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_FILE_OUTPUT_DIR}
                COMMAND ${SHADER_CONDUCTOR_CLI_EXECUTABLE} -I ${INPUT_SHADER_FILE} -T spirv -S ${SHADER_STAGE} -E main -O ${SPIRV}
                DEPENDS ${SHADER_FILE})
        list(APPEND SPIRV_BINARY_FILES ${SPIRV})
    endif ()
endforeach (SHADER_FILE)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(${PROJECT_NAME} Shaders SPIRV-Headers SPIRV-Cross)

target_link_libraries(${PROJECT_NAME} PUBLIC
        Vulkan::Vulkan
        glfw
        glm
        ${Boost_LIBRARIES}
        ${ASSIMP_LIBS_FIX}
        ${BULLET_LIBRARIES}
        ${SPRIV_CROSS_LIBRARIES}
        )

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} PUBLIC bcrypt)
endif ()