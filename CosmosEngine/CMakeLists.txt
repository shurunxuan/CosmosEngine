set(PROJECT_NAME CosmosEngine)
set(ENGINE_NAME ${PROJECT_NAME})

project(${PROJECT_NAME})

# Fix boost 1.71 find_package error
set(Boost_NO_BOOST_CMAKE 1)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Boost COMPONENTS log date_time log_setup filesystem thread regex chrono atomic random REQUIRED)
find_package(assimp REQUIRED)
# spirv-cross
foreach (SPRIV_CROSS_LIB ${SPRIV_CROSS_LIBRARIES})
    string(REPLACE "-" "_" SPRIV_CROSS_LIB_UNDERSCORE ${SPRIV_CROSS_LIB})
    find_package(${SPRIV_CROSS_LIB_UNDERSCORE} CONFIG REQUIRED)
endforeach()

link_directories(${ASSIMP_LIBRARY_DIRS})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(ASSIMP_LIBS_FIX ${ASSIMP_LIBRARY_DIRS}/libassimp.dylib)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(ASSIMP_LIBS_FIX assimp::assimp)
else ()
    set(ASSIMP_LIBS_FIX ${ASSIMP_LIBRARIES})
endif ()

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

include_directories(${FFMPEG_INCLUDE_DIRS})

add_compile_definitions(ENGINE_EXPORTS)

set(SOURCE_FILES
        App/App.cpp
        Core/Camera.cpp
        Core/Component.cpp
        Core/Material.cpp
        Core/Mesh.cpp
        Core/MeshRenderer.cpp
        Core/Object.cpp
        Core/Scene.cpp
        Core/Texture.cpp
        Core/Transform.cpp
        Input/GLFW/GLFWInputBackend.cpp
        Input/InputBackend.cpp
        Logging/Logging.cpp
        Rendering/CommandBuffer.cpp
        Rendering/ReflectionalShader.cpp
        Rendering/RenderingBackend.cpp
        Rendering/RenderingPipeline.cpp
        Rendering/Vulkan/ReflectionalSpirV.cpp
        Rendering/Vulkan/VulkanBackend.cpp
        Rendering/Vulkan/VulkanCommandBuffer.cpp
        Rendering/Vulkan/VulkanPipeline.cpp
        Threading/JobQueue.cpp
        Threading/JobSystem.cpp
        Physics/SphereCollider.cpp
        Physics/PhysicsSystem.cpp
        Audio/Decoder.cpp
        Audio/AudioBackend.cpp
        Audio/AudioPlayer.cpp
        Core/AudioSource.cpp)

set(HEADER_FILES
        Export.h
        App/App.h
        Core/Camera.h
        Core/Component.h
        Core/Material.h
        Core/Mesh.h
        Core/MeshRenderer.h
        Core/Object.h
        Core/Scene.h
        Core/Texture.h
        Core/Transform.h
        Input/GLFW/GLFWInputBackend.h
        Input/InputBackend.h
        Logging/Logging.h
        Rendering/CommandBuffer.h
        Rendering/ReflectionalShader.h
        Rendering/RenderingBackend.h
        Rendering/RenderingPipeline.h
        Rendering/Vulkan/ReflectionalSpirV.h
        Rendering/Vulkan/VulkanBackend.h
        Rendering/Vulkan/VulkanCommandBuffer.h
        Rendering/Vulkan/VulkanPipeline.h
        Threading/JobQueue.h
        Threading/JobSystem.h
        Physics/SphereCollider.h
        Physics/PhysicsSystem.h
        Audio/Decoder.h
        Audio/AudioBackend.h
        Audio/DefaultAudioBackend.h
        Audio/AudioPlayer.h
        Core/AudioSource.h)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(SYSTEM_SPECIFIC_SOURCE_FILES
            Audio/XAudio2/XAudio2Backend.cpp
            Audio/XAudio2/XAudio2Player.cpp
            Audio/XAudio2/XAudio2Callback.cpp)
    set(SYSTEM_SPECIFIC_HEADER_FILES
            Audio/XAudio2/XAudio2Backend.h
            Audio/XAudio2/XAudio2Player.h
            Audio/XAudio2/XAudio2Callback.h)

    set(SYSTEM_SPECIFIC_LIBRARIES
            bcrypt
            xaudio2)
    set(SYSTEM_SPECIFIC_INCLUDE_DIRS)

    set(SYSTEM_SPECIFIC_COMPILE_OPTIONS)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    # Objective-C Frameworks
    set(OBJC_FRAMEWORKS
            Foundation
            AudioToolbox
            CoreAudio
            AVFoundation
            )

    foreach (FRAMEWORK ${OBJC_FRAMEWORKS})
        set(OBJC_FRAMEWORK_STRING "${OBJC_FRAMEWORK_STRING} ${FRAMEWORK}")
        list(APPEND OBJC_LINK_FRAMEWORKS "-framework ${FRAMEWORK}")
    endforeach ()
    message("-- Using Objective-C Framework:${OBJC_FRAMEWORK_STRING}")

    set(SYSTEM_SPECIFIC_SOURCE_FILES
            Audio/CoreAudio/CoreAudioBackend.cpp
            Audio/CoreAudio/CoreAudioBackendBridge.m
            Logging/LoggingObjC.cpp
            Audio/CoreAudio/CoreAudioPlayer.cpp)
    set(SYSTEM_SPECIFIC_HEADER_FILES
            Audio/CoreAudio/CoreAudioBackend.h
            Audio/CoreAudio/CoreAudioBackendBridge.h
            Logging/LoggingObjC.h
            Audio/CoreAudio/CoreAudioPlayer.h)

    set(SYSTEM_SPECIFIC_LIBRARIES
            ${OBJC_LINK_FRAMEWORKS})
    set(SYSTEM_SPECIFIC_INCLUDE_DIRS)

    set(SYSTEM_SPECIFIC_COMPILE_OPTIONS
            "-fobjc-arc")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    find_package(OpenAL REQUIRED)

    set(SYSTEM_SPECIFIC_SOURCE_FILES
            Audio/OpenAL/OpenALBackend.cpp
            Audio/OpenAL/OpenALPlayer.cpp)
    set(SYSTEM_SPECIFIC_HEADER_FILES
            Audio/OpenAL/OpenALBackend.h
            Audio/OpenAL/OpenALPlayer.h)

    set(SYSTEM_SPECIFIC_LIBRARIES
            ${OPENAL_LIBRARY})
    set(SYSTEM_SPECIFIC_INCLUDE_DIRS
            ${OPENAL_INCLUDE_DIR})

    set(SYSTEM_SPECIFIC_COMPILE_OPTIONS)
endif ()

include_directories(${SYSTEM_SPECIFIC_INCLUDE_DIRS})

add_library(${PROJECT_NAME} SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES}
        ${SYSTEM_SPECIFIC_SOURCE_FILES}
        ${SYSTEM_SPECIFIC_HEADER_FILES})

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

add_dependencies(${PROJECT_NAME} Shaders)

target_link_libraries(${PROJECT_NAME} PUBLIC
        Vulkan::Vulkan
        glfw
        glm
        ${Boost_LIBRARIES}
        ${ASSIMP_LIBS_FIX}
        ${SPRIV_CROSS_LIBRARIES}
        ${FFMPEG_LIBRARIES}
        ${SYSTEM_SPECIFIC_LIBRARIES}
        )

target_compile_options(${PROJECT_NAME} PUBLIC ${SYSTEM_SPECIFIC_COMPILE_OPTIONS})