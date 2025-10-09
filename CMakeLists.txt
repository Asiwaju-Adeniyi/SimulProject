cmake_minimum_required(VERSION 3.12)
project(AsiwajuAdeniyi LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.c
)


add_executable(${PROJECT_NAME} ${SRC_FILES})


target_include_directories(${PROJECT_NAME}
    PRIVATE
        include
)


find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Link the libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        OpenGL::GL
        glfw
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
