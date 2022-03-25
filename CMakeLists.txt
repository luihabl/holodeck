cmake_minimum_required(VERSION 3.11)
project(holodeck)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform selection
option(PLATFORM_SDL2 "Use SDL2 platform" ON)
option(PLATFORM_GLFW "Use GLFW platform" OFF)

# Graphics backend selection
option(GRAPHICS_OPENGL "Use OpenGL" ON)

add_executable(holodeck
src/main.cpp
src/log.cpp
)

target_include_directories(holodeck PUBLIC
external/glm
external/stb
)

if(GRAPHICS_OPENGL)
    target_sources(holodeck PRIVATE 
    src/backends/graphics_opengl.cpp
    external/glad/src/glad.c
    )

    target_include_directories(holodeck PUBLIC
    external/glad/include
    )
endif()

if(PLATFORM_SDL2)
    find_package(SDL2 REQUIRED)

    target_sources(holodeck PRIVATE 
    src/backends/platform_sdl2.cpp
    )
    
    target_include_directories(holodeck PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_libraries(holodeck PUBLIC ${SDL2_LIBRARIES})
    add_compile_definitions(PLATFORM_SDL2)
    
    message("Using SDL ${SDL2_VERSION} platform")

elseif(PLATFORM_GLFW)
    find_package(GLFW3 REQUIRED)
    
    target_sources(holodeck PRIVATE 
    src/backends/platform_glfw.cpp
    )
    
    target_link_libraries(holodeck PUBLIC ${GLFW3_LIBRARIES})
    add_compile_definitions(PLATFORM_GLFW)
    
    message("Using GLFW ${GLFW3_VERSION} platform")

endif()