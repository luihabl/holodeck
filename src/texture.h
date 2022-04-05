#pragma once

#include "loader.h"

namespace holodeck
{
    struct Texture 
    {

        Texture() = default;
        // Texture(int w, int h, int n_comp, unsigned char * data);
        
        unsigned int id = 0;
        unsigned int tex_slot = 0;
        
        int w = 0, h = 0;

        void bind() const;
        void load(const Loader::DDSFile& dds);

        static Texture empty(int w, int h);
    };
}