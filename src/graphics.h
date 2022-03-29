#pragma once 

#include <glm/glm.hpp>

typedef void* (* GLLoaderFunction)(const char *name);

namespace holodeck {

    // struct Color;

    namespace Graphics {
        
        void load_gl_functions(GLLoaderFunction gl_loader);
        void setup_debug();
        void clear(const glm::vec3 & c);
        void clear(const glm::vec4 & c);

        int major_version();
        int minor_version();

        void init();

        // void set_blend_mode();
        // void viewport(int w, int h);
        // void viewport(int x, int y, int w, int h);
    }
}