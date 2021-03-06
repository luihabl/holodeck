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

    flags |= SDL_WINDOW_ALLOW_HIGHDPI;

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

    state.keyboard.init();
    Graphics::init();
}

void Platform::update()
{

    state.keyboard.update();

    SDL_GetWindowPosition(window, &state.win.x, &state.win.y); 
    SDL_GetWindowSize(window, &state.win.w, &state.win.h); 

    #ifdef GRAPHICS_OPENGL
    SDL_GL_GetDrawableSize(window, &state.win.drawable_w, &state.win.drawable_h); 
    #endif

    SDL_GetGlobalMouseState(&state.mouse.global_x, &state.mouse.global_y);

    int mouse_x, mouse_y;
    mouse_x = state.mouse.global_x - state.win.x;
    mouse_y = state.mouse.global_y - state.win.y;

    if (state.mouse.relative)
    {
        SDL_GetRelativeMouseState(&state.mouse.offset_x, &state.mouse.offset_y);
    }
    else
    {
        state.mouse.offset_x = mouse_x - state.mouse.x;
        state.mouse.offset_y = mouse_y - state.mouse.y;
    }

    state.mouse.x = mouse_x;
    state.mouse.y = mouse_y;

    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_QUIT)
        {
            if(config.callbacks.on_exit) 
                config.callbacks.on_exit();
            continue;
        }
        
        else if (event.type == SDL_KEYDOWN)
        {
            if(event.key.repeat == 0)
                state.keyboard.on_press((Key) event.key.keysym.scancode);
        }
        else if (event.type == SDL_KEYUP)
        {
            if(event.key.repeat == 0)
                state.keyboard.on_release((Key) event.key.keysym.scancode);
        }


        // else if(event.type == SDL_MOUSEBUTTONDOWN)
        // {

        // }
        // else if(event.type == SDL_MOUSEBUTTONUP)
        // {

        // }

    //     case SDL_WINDOWEVENT: {
    //   SDL_WindowEvent e = event->window;
    //   switch (e.event) {
    //     case SDL_WINDOWEVENT_ENTER:
    //       std::cout << "window enter\n";
    //       break;
    //     case SDL_WINDOWEVENT_LEAVE:
    //       std::cout << "window leave\n";
    //       break;
    //     default:
    //       break;
    //   }
        // else if(event.type == SDL_WINDOWEVENT)
        // {
        //     if(event.window.type == SDL_WINDOWEVENT_ENTER)
        //     {
        //         state.mouse.offset_x = 0;
        //         state.mouse.offset_y = 0;
        //     }
        // }

        else if(event.type == SDL_MOUSEMOTION)
        {
            if(config.callbacks.on_mouse_movement)
                config.callbacks.on_mouse_movement();
        }

    }
}

void Platform::set_mouse_pos(int x, int y)
{
    SDL_WarpMouseInWindow(window, x, y);
}

void Platform::set_mouse_focus()
{
    SDL_SetWindowInputFocus(window);
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

void Platform::hide_mouse()
{
    state.mouse.visible = false;
    SDL_ShowCursor(SDL_DISABLE);
}

void Platform::show_mouse()
{
    state.mouse.visible = true;
    SDL_ShowCursor(SDL_ENABLE);
}

void Platform::relative_mouse(bool activate)
{
    if(activate && !state.mouse.relative)
    {
        state.mouse.relative = true;
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    else if(!activate && state.mouse.relative)
    {
        state.mouse.relative = false;
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }   
}


#endif