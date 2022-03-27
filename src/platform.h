#pragma once

#include <string>
#include <functional>

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

        std::function<void()> on_exit = nullptr;

    };


    class Platform
    {
        public:
        void init(const PlatformConfig& config);
        void update();
        void swap_buffers();


        void terminate();

        private:
        PlatformConfig config;

    };
}