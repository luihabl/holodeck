#pragma once

#include "loader.h"
#include <memory>

namespace holodeck
{
    struct Texture 
    {
        using Ref = std::shared_ptr<Texture>;

        unsigned int id = 0;
        unsigned int tex_slot = 0;
        
        int w = 0, h = 0;

        void bind() const;

        static Ref from_dds(const Loader::DDSFile& dds);
        static Ref from_png(const Loader::PNGFile& png);
    
    };
}