cmake_minimum_required(VERSION 3.11)
project(holodeck)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GLFW3 REQUIRED)

include_directories(external/glm)

add_executable(holodeck
src/main.cpp

external/glad/src/glad.c
)

target_include_directories(holodeck PUBLIC
external/glad/include
${GLFW3_INCLUDE_DIR}
)

target_link_libraries(holodeck PUBLIC
${GLFW3_LIBRARIES}
)