#include "loader.h"

#include "file.h"

using namespace holodeck::Loader;

OBJFile OBJFile::load(const std::filesystem::path& file_path)
{

    File::Raw file(file_path);
    file.open("r");

    char c[512];
    
    OBJFile obj;
    
    obj.positions = {};
    obj.normals = {};
    obj.uv = {};
    obj.indices = {};

    while(!file.eof())
    {
        fgets(c, 512, file.fp);
        

        if(c[0] == 'v' && c[1] == 'n')
        {
            float x, y, z;
            sscanf(c, "vn %f %f %f", &x, &y, &z);
            obj.normals.push_back({x, y, z});
        }

        else if(c[0] == 'v' && c[1] == ' ')
        {   
            float x, y, z, ux, uy, uz;
            sscanf(c, "v %f %f %f %f %f %f", &x, &y, &z, &ux, &uy, &uz);
            obj.positions.push_back({x, y, z});
        }
        
        else if(c[0] == 'f' && c[1] == ' ')
        {   
            unsigned i0, i1, i2, ignore;
            sscanf(c, "f %u//%u %u//%u %u//%u", &i0, &ignore, &i1, &ignore, &i2, &ignore);
            obj.indices.insert(obj.indices.end(), {i0 - 1, i1 - 1, i2 - 1});
        }
        
    }

    return obj;
}


void OBJFile::clear()
{
    std::filesystem::path path = "";

    std::vector<glm::vec3> positions = {};
    std::vector<glm::vec3> normals = {};
    std::vector<glm::vec2> uv = {};
    std::vector<unsigned> indices = {};
}
