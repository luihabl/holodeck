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
    private:
        void draw();
    };
}