#pragma once

#include "platform.h"
#include "gui.h"

namespace holodeck
{
    class App
    {
    public:

        App(const PlatformConfig& config);

        void run();

    private:
        Platform m_platform;
        bool m_quit = false;
        GUI m_gui;

    };
}