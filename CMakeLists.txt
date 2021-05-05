cmake_minimum_required(VERSION 3.3)
project(Snake LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.c")

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 99)

target_link_libraries(${PROJECT_NAME}
    PRIVATE SDL2
    PRIVATE SDL2_ttf
    PRIVATE m
)

