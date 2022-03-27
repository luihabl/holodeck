
#include "platform.h"
#include "graphics.h"

using namespace holodeck;

int main()
{
    bool quit = false;

    Platform platform;
    PlatformConfig config;
    config.w = 800;
    config.h = 600;
    config.name = "holodeck";
    config.on_exit = [&quit]() { quit = true; };

    platform.init(config);

    while(!quit)
    {
        platform.update();
        Graphics::clear(glm::vec3(0, 0.1f, 0.1f));

        platform.swap_buffers();
        
    }

    return 0;
}