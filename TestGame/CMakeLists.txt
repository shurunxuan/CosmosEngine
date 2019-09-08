set(PROJECT_NAME TestGame)

project(${PROJECT_NAME})

set(SOURCE_FILES
        main.cpp
        TestGameApp/TestGameApp.cpp
        Scripts/ObjectMovement.cpp Scripts/CameraMovement.cpp)

set(HEADER_FILES
        TestGameApp/TestGameApp.h
        Scripts/ObjectMovement.h Scripts/CameraMovement.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

add_dependencies(${PROJECT_NAME} CosmosEngine)

target_include_directories(${PROJECT_NAME} PRIVATE ..)

target_link_libraries(${PROJECT_NAME} PRIVATE CosmosEngine)