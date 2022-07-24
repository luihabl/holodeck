#pragma once

#include "input.h"

#include <string>
#include <functional>
#include <cstdint>

namespace holodeck
{
    struct PlatformConfig
    {
        PlatformConfig(int _w, int _h, std::string _name = "") : w(_w), h(_h), name(_name) {}
        PlatformConfig() = default;

        int w = 800, h = 600;
        int x=-1, y=-1;
        std::string name = "";
        bool use_vsync = true;
        bool use_opengl = true;
        bool resizable = true;

        struct Callbacks
        {
            std::function<void()> on_exit = nullptr;
            std::function<void()> on_mouse_movement = nullptr;
        } callbacks;
    };

    struct PlatformState
    {
        KeyboardState keyboard;
        MouseState mouse;
        WindowState win;

        bool mouse_inside()
        {
            return (mouse.x > 0 && mouse.x < win.w) && (mouse.y > 0 && mouse.y < win.h);
        }
    };


    class Platform
    {
        public:
        
        void init(const PlatformConfig& config);
        void update();
        void swap_buffers();
        uint64_t get_time_ms();

        PlatformState state;

        void terminate();

        void set_mouse_pos(int x, int y);
        void set_mouse_focus();
        void hide_mouse();
        void show_mouse();
        void relative_mouse(bool activate = true);

        PlatformConfig::Callbacks& callbacks() { return config.callbacks; }

        PlatformConfig config;

    };
}