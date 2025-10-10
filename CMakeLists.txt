cmake_minimum_required(VERSION 3.10)
project(AsiwajuAdeniyi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(include)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/solver.cpp
    src/renderer.cpp
    src/glad.c        
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        ${OPENGL_LIBRARIES}
        Threads::Threads
)
