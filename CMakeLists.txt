cmake_minimum_required(VERSION 3.11)
project(holodeck)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform selection
option(PLATFORM_SDL2 "Use SDL2 platform" OFF)
option(PLATFORM_GLFW "Use GLFW platform" ON)

# Graphics backend selection
option(GRAPHICS_OPENGL "Use OpenGL" ON)

add_executable(holodeck
src/camera.h
src/constants.h
src/main.cpp
src/log.h
src/log.cpp
src/file.h
src/file.cpp
src/graphics.h
src/input.h
src/model.h
src/model.cpp
src/loader.cpp
src/shader.h
src/texture.h
src/gui.cpp
src/gui.h
)

target_include_directories(holodeck PUBLIC
external/glm
external/stb
)

## Dear ImGui ----
target_include_directories(holodeck PUBLIC
external/imgui
external/imgui/backends
)
target_sources(holodeck PRIVATE
external/imgui/imgui.cpp
external/imgui/imgui.cpp
external/imgui/imgui_draw.cpp
external/imgui/imgui_tables.cpp
external/imgui/imgui_widgets.cpp
external/imgui/imgui_demo.cpp
)

## OpenGL ----
if(GRAPHICS_OPENGL)
    target_sources(holodeck PRIVATE 
    src/backends/graphics_opengl.cpp
    external/glad/src/glad.c
    external/imgui/backends/imgui_impl_opengl3.cpp
    )

    target_include_directories(holodeck PUBLIC
    external/glad/include
    )
    add_compile_definitions(GRAPHICS_OPENGL)
endif()

## SDL ----
if(PLATFORM_SDL2)
    find_package(SDL2 REQUIRED)

    target_sources(holodeck PRIVATE 
    src/backends/platform_sdl2.cpp
    external/imgui/backends/imgui_impl_sdl.cpp
    )
    
    target_include_directories(holodeck PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_libraries(holodeck PUBLIC ${SDL2_LIBRARIES})
    add_compile_definitions(PLATFORM_SDL2)
    
    message("Using SDL ${SDL2_VERSION} platform")

## GLFW ----
elseif(PLATFORM_GLFW)
    find_package(GLFW3 REQUIRED)
    
    target_sources(holodeck PRIVATE 
    src/backends/platform_glfw.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    )
    
    target_link_libraries(holodeck PUBLIC glfw)
    add_compile_definitions(PLATFORM_GLFW)
    
    message("Using GLFW ${GLFW3_VERSION} platform")

endif()
