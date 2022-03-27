#ifdef PLATFORM_SDL2

#include "../platform.h"
#include "../graphics.h"
#include "../log.h"

#include <SDL.h>
#include <cstdint>

#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

using namespace holodeck;

namespace 
{
    SDL_Window* window = nullptr;
    SDL_GLContext context;
}

void Platform::init(const PlatformConfig& _config)
{
    config = _config;

    uint32_t flags = 0;

    SDL_Init(SDL_INIT_VIDEO);
    if(config.use_opengl)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_VERSION_MAJOR);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_VERSION_MINOR);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

        SDL_GL_SetSwapInterval(config.use_vsync);

        flags |= SDL_WINDOW_OPENGL;
    }

    if(config.resizable)
    {
        flags |= SDL_WINDOW_RESIZABLE;
    }    

    int x = 0;
    if(config.x < 0)
        x = SDL_WINDOWPOS_UNDEFINED;

    int y = 0;
    if(config.y < 0)
        y = SDL_WINDOWPOS_UNDEFINED;
    
    window = SDL_CreateWindow(config.name.c_str(), x, y, config.w, config.h, flags);

    SDL_version sdl_version;
    SDL_GetVersion(&sdl_version);
    Log::info("Platform: SDL %i.%i.%i", sdl_version.major, sdl_version.minor, sdl_version.patch);

    if(config.use_opengl)
    {
        context = SDL_GL_CreateContext( window );
        SDL_GL_MakeCurrent(window, context);
        Graphics::load_gl_functions(SDL_GL_GetProcAddress);
    }

    // Graphics::setup_debug();
}

void Platform::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_QUIT)
        {
            if(config.on_exit) 
                config.on_exit();
            continue;
        }
    }
}

void Platform::swap_buffers()
{
    SDL_GL_SwapWindow(window);
}


#endif