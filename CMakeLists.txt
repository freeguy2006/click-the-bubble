cmake_minimum_required(VERSION 3.10)
project(OOP_Raylib_Lab CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
)

if(NOT SOURCES)
    message(FATAL_ERROR "No source files were found in ${SRC_DIR}")
endif()

find_package(raylib 5 QUIET)

if(NOT raylib_FOUND)
    include(FetchContent)

    message(STATUS "raylib not found locally. Fetching raylib from GitHub...")
    set(FETCHCONTENT_QUIET OFF)

    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(raylib)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Cocoa")
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/resources")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/resources"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
        COMMENT "Copying assets to output directory"
    )
endif()
