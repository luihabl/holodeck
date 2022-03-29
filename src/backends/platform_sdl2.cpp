#ifdef PLATFORM_SDL2

#include "../platform.h"
#include "../graphics.h"
#include "../log.h"

#include <SDL.h>

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
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, Graphics::major_version());
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, Graphics::minor_version());
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

    input.keyboard.init();
    Graphics::init();
}

void Platform::update()
{

    input.keyboard.update();

    int win_x, win_y;
    SDL_GetWindowPosition(window, &win_x, &win_y); 
    SDL_GetGlobalMouseState(&input.mouse.global_x, &input.mouse.global_y);
    input.mouse.x = input.mouse.global_x - win_x;
    input.mouse.y = input.mouse.global_y - win_y;

    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_QUIT)
        {
            if(config.on_exit) 
                config.on_exit();
            continue;
        }
        
        else if (event.type == SDL_KEYDOWN)
        {
            if(event.key.repeat == 0)
                input.keyboard.on_press((Key) event.key.keysym.scancode);
        }
        else if (event.type == SDL_KEYUP)
        {
            if(event.key.repeat == 0)
                input.keyboard.on_release((Key) event.key.keysym.scancode);
        }


        // else if(event.type == SDL_MOUSEBUTTONDOWN)
        // {

        // }
        // else if(event.type == SDL_MOUSEBUTTONUP)
        // {

        // }





    }
}

void Platform::swap_buffers()
{
    SDL_GL_SwapWindow(window);
}

uint64_t Platform::get_time_ms()
{
    return SDL_GetTicks64();
}

void Platform::terminate()
{
	SDL_DestroyWindow(window);
	window = nullptr;
    SDL_GL_DeleteContext(context);
	SDL_Quit();
}


#endif