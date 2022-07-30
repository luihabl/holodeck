#pragma once

#include "platform.h"
#include "gui.h"
#include "scene.h"

namespace holodeck
{
    class App
    {
    public:

        App(const PlatformConfig& config);

        void run();

    private:
        Scene m_scene;
        Platform m_platform;
        bool m_quit = false;
        GUI m_gui;

    };
}