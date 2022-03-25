#pragma once 

typedef void* (* GLLoaderFunction)(const char *name);

namespace holodeck {

    // struct Color;

    namespace Graphics {
        void load_gl_functions(GLLoaderFunction gl_loader);
        void setup_debug();
        
        // void clear(const Color & c);
        // void set_blend_mode();
        // void viewport(int w, int h);
        // void viewport(int x, int y, int w, int h);
    }
}