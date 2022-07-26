#ifdef PLATFORM_GLFW

#include "../platform.h"
#include "../graphics.h"
#include "../log.h"
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace holodeck;

namespace 
{
    Key glfwKeyMap[GLFW_KEY_LAST + 1];

    void set_key_map()
    {
        glfwKeyMap[GLFW_KEY_SPACE] = Key::Space;
        glfwKeyMap[GLFW_KEY_APOSTROPHE] = Key::Apostrophe;
        glfwKeyMap[GLFW_KEY_COMMA] = Key::Comma;
        glfwKeyMap[GLFW_KEY_MINUS] = Key::Minus;
        glfwKeyMap[GLFW_KEY_PERIOD] = Key::Period;
        glfwKeyMap[GLFW_KEY_SLASH] = Key::Slash;
        glfwKeyMap[GLFW_KEY_0] = Key::Key0;
        glfwKeyMap[GLFW_KEY_1] = Key::Key1;
        glfwKeyMap[GLFW_KEY_2] = Key::Key2;
        glfwKeyMap[GLFW_KEY_3] = Key::Key3;
        glfwKeyMap[GLFW_KEY_4] = Key::Key4;
        glfwKeyMap[GLFW_KEY_5] = Key::Key5;
        glfwKeyMap[GLFW_KEY_6] = Key::Key6;
        glfwKeyMap[GLFW_KEY_7] = Key::Key7;
        glfwKeyMap[GLFW_KEY_8] = Key::Key8;
        glfwKeyMap[GLFW_KEY_9] = Key::Key9;
        glfwKeyMap[GLFW_KEY_SEMICOLON] = Key::Semicolon;
        glfwKeyMap[GLFW_KEY_EQUAL] = Key::Equals;
        glfwKeyMap[GLFW_KEY_A] = Key::A;
        glfwKeyMap[GLFW_KEY_B] = Key::B;
        glfwKeyMap[GLFW_KEY_C] = Key::C;
        glfwKeyMap[GLFW_KEY_D] = Key::D;
        glfwKeyMap[GLFW_KEY_E] = Key::E;
        glfwKeyMap[GLFW_KEY_F] = Key::F;
        glfwKeyMap[GLFW_KEY_G] = Key::G;
        glfwKeyMap[GLFW_KEY_H] = Key::H;
        glfwKeyMap[GLFW_KEY_I] = Key::I;
        glfwKeyMap[GLFW_KEY_J] = Key::J;
        glfwKeyMap[GLFW_KEY_K] = Key::K;
        glfwKeyMap[GLFW_KEY_L] = Key::L;
        glfwKeyMap[GLFW_KEY_M] = Key::M;
        glfwKeyMap[GLFW_KEY_N] = Key::N;
        glfwKeyMap[GLFW_KEY_O] = Key::O;
        glfwKeyMap[GLFW_KEY_P] = Key::P;
        glfwKeyMap[GLFW_KEY_Q] = Key::Q;
        glfwKeyMap[GLFW_KEY_R] = Key::R;
        glfwKeyMap[GLFW_KEY_S] = Key::S;
        glfwKeyMap[GLFW_KEY_T] = Key::T;
        glfwKeyMap[GLFW_KEY_U] = Key::U;
        glfwKeyMap[GLFW_KEY_V] = Key::V;
        glfwKeyMap[GLFW_KEY_W] = Key::W;
        glfwKeyMap[GLFW_KEY_X] = Key::X;
        glfwKeyMap[GLFW_KEY_Y] = Key::Y;
        glfwKeyMap[GLFW_KEY_Z] = Key::Z;
        glfwKeyMap[GLFW_KEY_LEFT_BRACKET] = Key::LeftBracket;
        glfwKeyMap[GLFW_KEY_BACKSLASH] = Key::BackSlash;
        glfwKeyMap[GLFW_KEY_RIGHT_BRACKET] = Key::RightBracket;
        glfwKeyMap[GLFW_KEY_GRAVE_ACCENT] = Key::Grave;
        // glfwKeyMap[GLFW_KEY_WORLD_1] = Key::1;
        // glfwKeyMap[GLFW_KEY_WORLD_2] = Key::2;
        glfwKeyMap[GLFW_KEY_ESCAPE] = Key::Escape;
        glfwKeyMap[GLFW_KEY_ENTER] = Key::Return;
        glfwKeyMap[GLFW_KEY_TAB] = Key::Tab;
        glfwKeyMap[GLFW_KEY_BACKSPACE] = Key::BackSpace;
        glfwKeyMap[GLFW_KEY_INSERT] = Key::Insert;
        glfwKeyMap[GLFW_KEY_DELETE] = Key::Delete;
        glfwKeyMap[GLFW_KEY_RIGHT] = Key::Right;
        glfwKeyMap[GLFW_KEY_LEFT] = Key::Left;
        glfwKeyMap[GLFW_KEY_DOWN] = Key::Down;
        glfwKeyMap[GLFW_KEY_UP] = Key::Up;
        glfwKeyMap[GLFW_KEY_PAGE_UP] = Key::PageUp;
        glfwKeyMap[GLFW_KEY_PAGE_DOWN] = Key::PageDown;
        glfwKeyMap[GLFW_KEY_HOME] = Key::Home;
        glfwKeyMap[GLFW_KEY_END] = Key::End;
        glfwKeyMap[GLFW_KEY_CAPS_LOCK] = Key::CapsLock;
        glfwKeyMap[GLFW_KEY_SCROLL_LOCK] = Key::ScrollLock;
        glfwKeyMap[GLFW_KEY_NUM_LOCK] = Key::LockingNumLock;
        glfwKeyMap[GLFW_KEY_PRINT_SCREEN] = Key::PrintScreen;
        glfwKeyMap[GLFW_KEY_PAUSE] = Key::Pause;
        glfwKeyMap[GLFW_KEY_F1] = Key::F1;
        glfwKeyMap[GLFW_KEY_F2] = Key::F2;
        glfwKeyMap[GLFW_KEY_F3] = Key::F3;
        glfwKeyMap[GLFW_KEY_F4] = Key::F4;
        glfwKeyMap[GLFW_KEY_F5] = Key::F5;
        glfwKeyMap[GLFW_KEY_F6] = Key::F6;
        glfwKeyMap[GLFW_KEY_F7] = Key::F7;
        glfwKeyMap[GLFW_KEY_F8] = Key::F8;
        glfwKeyMap[GLFW_KEY_F9] = Key::F9;
        glfwKeyMap[GLFW_KEY_F10] = Key::F10;
        glfwKeyMap[GLFW_KEY_F11] = Key::F11;
        glfwKeyMap[GLFW_KEY_F12] = Key::F12;
        glfwKeyMap[GLFW_KEY_F13] = Key::F13;
        glfwKeyMap[GLFW_KEY_F14] = Key::F14;
        glfwKeyMap[GLFW_KEY_F15] = Key::F15;
        glfwKeyMap[GLFW_KEY_F16] = Key::F16;
        glfwKeyMap[GLFW_KEY_F17] = Key::F17;
        glfwKeyMap[GLFW_KEY_F18] = Key::F18;
        glfwKeyMap[GLFW_KEY_F19] = Key::F19;
        glfwKeyMap[GLFW_KEY_F20] = Key::F20;
        glfwKeyMap[GLFW_KEY_F21] = Key::F21;
        glfwKeyMap[GLFW_KEY_F22] = Key::F22;
        glfwKeyMap[GLFW_KEY_F23] = Key::F23;
        glfwKeyMap[GLFW_KEY_F24] = Key::F24;
        // glfwKeyMap[GLFW_KEY_F25] = Key::F25;
        glfwKeyMap[GLFW_KEY_KP_0] = Key::Kp0;
        glfwKeyMap[GLFW_KEY_KP_1] = Key::Kp1;
        glfwKeyMap[GLFW_KEY_KP_2] = Key::Kp2;
        glfwKeyMap[GLFW_KEY_KP_3] = Key::Kp3;
        glfwKeyMap[GLFW_KEY_KP_4] = Key::Kp4;
        glfwKeyMap[GLFW_KEY_KP_5] = Key::Kp5;
        glfwKeyMap[GLFW_KEY_KP_6] = Key::Kp6;
        glfwKeyMap[GLFW_KEY_KP_7] = Key::Kp7;
        glfwKeyMap[GLFW_KEY_KP_8] = Key::Kp8;
        glfwKeyMap[GLFW_KEY_KP_9] = Key::Kp9;
        glfwKeyMap[GLFW_KEY_KP_DECIMAL] = Key::KpDecimal;
        glfwKeyMap[GLFW_KEY_KP_DIVIDE] = Key::KpDivide;
        glfwKeyMap[GLFW_KEY_KP_MULTIPLY] = Key::KpMultiply;
        glfwKeyMap[GLFW_KEY_KP_SUBTRACT] = Key::KpMemSubtract;
        glfwKeyMap[GLFW_KEY_KP_ADD] = Key::KpMemAdd;
        glfwKeyMap[GLFW_KEY_KP_ENTER] = Key::KpEnter;
        glfwKeyMap[GLFW_KEY_KP_EQUAL] = Key::KpEquals;
        glfwKeyMap[GLFW_KEY_LEFT_SHIFT] = Key::LShift;
        glfwKeyMap[GLFW_KEY_LEFT_CONTROL] = Key::LCtrl;
        glfwKeyMap[GLFW_KEY_LEFT_ALT] = Key::LAlt;
        // glfwKeyMap[GLFW_KEY_LEFT_SUPER] = Key::SUPER;
        glfwKeyMap[GLFW_KEY_RIGHT_SHIFT] = Key::RShift;
        glfwKeyMap[GLFW_KEY_RIGHT_CONTROL] = Key::RCtrl;
        glfwKeyMap[GLFW_KEY_RIGHT_ALT] = Key::RAlt;
        // glfwKeyMap[GLFW_KEY_RIGHT_SUPER] = Key::SUPER;
        glfwKeyMap[GLFW_KEY_MENU] = Key::Menu;
    }

    GLFWwindow* window = nullptr;

    void key_pressed_callback(GLFWwindow* _window, int key, int scancode, int action, int mods)
    {
        Platform* p = (Platform*)glfwGetWindowUserPointer(_window);

        if(action == GLFW_PRESS)
            p->state.keyboard.on_press(glfwKeyMap[key]);
        else if (action == GLFW_RELEASE)
            p->state.keyboard.on_release(glfwKeyMap[key]);
    }

    void mouse_move_callback(GLFWwindow* _window, double xpos, double ypos)
    {
        Platform* p = (Platform*)glfwGetWindowUserPointer(_window);

        if(p->config.callbacks.on_mouse_movement)
            p->config.callbacks.on_mouse_movement();
    }

    void window_focus_callback(GLFWwindow* _window, int focused)
    {
        Platform* p = (Platform*)glfwGetWindowUserPointer(_window);

        if(p->config.callbacks.on_mouse_movement)
            p->config.callbacks.on_focus(focused == GLFW_TRUE);
    }

    void mouse_click_callback(GLFWwindow* _window, int button, int action, int mods)
    {
        Platform* p = (Platform*)glfwGetWindowUserPointer(_window);

        if(p->config.callbacks.on_mouse_click)
        {
            int button_code = -1;
            if (button == GLFW_MOUSE_BUTTON_LEFT)
                button_code = 0;
            else if (button == GLFW_MOUSE_BUTTON_RIGHT)
                button_code = 1;
            else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
                button_code = 2;

            int is_pressed = -1;
            if(action == GLFW_PRESS)
                is_pressed = 1;
            else if(action == GLFW_RELEASE)
                is_pressed = 0;

            if(button_code < 0|| is_pressed < 0)
                return;

            p->config.callbacks.on_mouse_click(button_code, is_pressed);
        }


        // if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        //     popup_menu();
    }
}

void Platform::init(const PlatformConfig& _config)
{
    config = _config;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Graphics::major_version());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Graphics::minor_version());
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwSwapInterval(config.use_vsync);

    if(config.resizable)
    {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    }
    else 
    {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    window = glfwCreateWindow(config.w, config.h, config.name.c_str(), nullptr, nullptr);

    Log::info("Platform: GLFW %i.%i.%i", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);

    if(config.use_opengl)
    {
        glfwMakeContextCurrent(window);
        Graphics::load_gl_functions((GLLoaderFunction) glfwGetProcAddress);
    }

    state.keyboard.init();
    Graphics::init();

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_pressed_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);
    glfwSetWindowFocusCallback(window, window_focus_callback);
    set_key_map();
}

void Platform::update()
{
    state.keyboard.update();

    glfwGetWindowPos(window, &state.win.x, &state.win.y);
    glfwGetWindowSize(window, &state.win.w, &state.win.h); 

    glfwGetFramebufferSize(window, &state.win.drawable_w, &state.win.drawable_h);

    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    state.mouse.global_x = std::floor(mouse_x) + state.win.x;
    state.mouse.global_y = std::floor(mouse_y) + state.win.y;

    state.mouse.offset_x = mouse_x - state.mouse.x;
    state.mouse.offset_y = mouse_y - state.mouse.y;

    state.mouse.x = std::floor(mouse_x);
    state.mouse.y = std::floor(mouse_y);

    glfwPollEvents();

    if(glfwWindowShouldClose(window))
    {
        if(config.callbacks.on_exit)
            config.callbacks.on_exit();
    }
}

void Platform::set_mouse_pos(int x, int y)
{
    glfwSetCursorPos(window, (double)x, (double)y);
}

void Platform::set_mouse_focus()
{
    glfwFocusWindow(window);
}

uint64_t Platform::get_time_ms()
{
    return static_cast<uint64_t>(std::round(glfwGetTime() / 1.0e3));
}

void Platform::relative_mouse(bool activate)
{
    if(activate && !state.mouse.relative)
    {
        state.mouse.relative = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else if(!activate && state.mouse.relative)
    {
        state.mouse.relative = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }  
}

void Platform::hide_mouse()
{
    state.mouse.visible = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

}

void Platform::show_mouse()
{
    state.mouse.visible = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void Platform::swap_buffers()
{
    glfwSwapBuffers(window);
}

void Platform::terminate()
{
    glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();
}


#endif