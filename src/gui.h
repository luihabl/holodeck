#pragma once

#include "platform.h"
#include "scene.h"

namespace holodeck
{
    class GUI
    {
    
    public:
        void initialize(const Platform& platform);
        void render(Scene* scene);
        void shutdown();

        void use_mouse(bool use);
        bool wants_mouse();
        bool wants_keyboard();

    private:
        void draw(Scene* scene);
        void draw_main_menu();
    };
}