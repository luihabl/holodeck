#pragma once

#include "platform.h"

namespace holodeck
{
    class GUI
    {
    
    public:
        void initialize(const Platform& platform);
        void render();
        void shutdown();

        void use_mouse(bool use);
        bool wants_mouse();

    private:
        void draw();
    };
}