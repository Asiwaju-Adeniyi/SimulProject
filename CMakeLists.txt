cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(OpenGLProject
    src/main.cpp
    src/glad.c
)

target_link_libraries(OpenGLProject
    glfw
    ${OPENGL_LIBRARIES}
)
