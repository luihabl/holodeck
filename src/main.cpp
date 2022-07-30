
#include "platform.h"
#include "app.h"

using namespace holodeck;

int main()
{   
    PlatformConfig config;
    config.w = 1920;
    config.h = 1080;
    config.name = "holodeck";

    App app(config);
    app.run();

    return 0;
}