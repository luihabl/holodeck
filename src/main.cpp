
#include "platform.h"

using namespace holodeck;

int main()
{
    Platform p;

    p.init(PlatformConfig(256, 128, "holodeck"));

    return 0;
}