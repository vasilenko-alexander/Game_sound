cmake_minimum_required(VERSION 3.2)

project(Game_sound)

set(CMAKE_CXX_STANDARD 11)
set(HEADERS ${CMAKE_SOURCE_DIR}/src/headers)
set(PROJECT_LIBS_DIR ${CMAKE_SOURCE_DIR}/lib)
set(SOURCES_DIR ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_LIBS_DIR})
file(GLOB SOURCES ${SOURCES_DIR}/game.cpp)

file(GLOB LIB_SOURCES ${SOURCES_DIR}/engine.cpp
                      ${SOURCES_DIR}/matrix.cpp
                      ${SOURCES_DIR}/image_loader.cpp
                      ${SOURCES_DIR}/operators.cpp
                      ${SOURCES_DIR}/shader_loader.cpp
                      ${SOURCES_DIR}/sound_loader.cpp)

include_directories(${HEADERS})

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

link_directories(${PROJECT_LIBS_DIR})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(ENGINE_LIB_NAME engined)
    add_library(${ENGINE_LIB_NAME} SHARED ${LIB_SOURCES})
else(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(ENGINE_LIB_NAME engine)
    add_library(${ENGINE_LIB_NAME} SHARED ${LIB_SOURCES})
endif()
    set(ENGINE_LINK_LIB -lSDL2 -lGL -lGLEW)
    target_link_libraries(${ENGINE_LIB_NAME} ${ENGINE_LINK_LIB})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${ENGINE_LIB_NAME})