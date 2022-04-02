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
            OBJFile() = default;
            OBJFile(const std::filesystem::path& file_path);

            std::filesystem::path path;

            std::vector<glm::vec4> positions;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> uv;
            std::vector<unsigned> indices;

        };



    }
}