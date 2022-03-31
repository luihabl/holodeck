#include <string>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "file.h"
#include "log.h"

#include <cstdio>

using namespace holodeck; 

std::string File::load_txt(std::string file_path) {
    std::ifstream in(file_path, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    
    Log::error("File does not exist: %s", file_path.c_str());
    return "";
}


unsigned char * File::load_image(std::string file_path, int *w, int *h, int *comp) {
    return stbi_load(file_path.c_str(), w, h, comp, 0);
}

void File::free_image(unsigned char * data) {
    stbi_image_free(data);
}

File::Raw::~Raw()
{
    close();
}

void File::Raw::open(const std::string& mode)
{
    fp = fopen(path.c_str(), mode.c_str());
    if (!fp)
    {
        Log::error("File does not exist: %s", path.c_str());
        return;
    }
}

void File::Raw::close()
{
    fclose(fp);
}