#pragma once

#include <string>

namespace holodeck
{
    struct PlatformConfig
    {
        int w, h;
        int x=-1, y=-1;
        std::string name = "";
        bool use_vsync = true;
        bool use_opengl = true;
        bool resizable = true;

        PlatformConfig(int _w, int _h, std::string _name = "") : w(_w), h(_h), name(_name) {}
    };


    class Platform
    {
        public:
        void init(const PlatformConfig& config);
        void terminate();
    };
}