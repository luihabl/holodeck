#pragma once 

#include <glm/glm.hpp>

#include <vector>
#include <filesystem>

namespace holodeck
{
    namespace Loader
    {
        struct OBJFile
        {
            std::filesystem::path path;

            std::vector<glm::vec3> positions;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> uv;
            std::vector<unsigned> indices;

            static OBJFile load(const std::filesystem::path& file_path);
            void clear();

        };



    }
}