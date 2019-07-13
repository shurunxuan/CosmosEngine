set(PROJECT_NAME CosmosEngine)
set(ENGINE_NAME ${PROJECT_NAME})

project(${PROJECT_NAME})

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(Boost COMPONENTS log REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_compile_definitions(ENGINE_EXPORTS)

set(SOURCE_FILES
        App/App.cpp
        Rendering/RenderingBackend.cpp
        Rendering/Vulkan/VulkanBackend.cpp
        Logging/Logging.cpp)

set(HEADER_FILES
        App/App.h
        Rendering/RenderingBackend.h
        Rendering/Vulkan/VulkanBackend.h
        Logging/Logging.h)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})

add_definitions( -DBOOST_ALL_NO_LIB )
add_definitions( -DBOOST_ALL_DYN_LINK )

target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan glfw glm ${Boost_LIBRARIES})