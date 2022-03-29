#pragma once 

#include "shader.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

namespace holodeck
{
    class Model
    {
    public:
        Model() = default;

        void init();
        void clear();
        void render(Shader* shader = nullptr);

        void load(const std::vector<glm::vec3> & pos, const std::vector<unsigned> _indices);
        void load(const std::vector<glm::vec4> & pos, const std::vector<unsigned> _indices);

        std::string name;
        glm::mat4 transform;

        static Model unit_cube();
    
    private:

        struct Vertex
        {
            glm::vec4 pos;
            glm::vec3 normal;
            glm::vec2 uv;
        };

        std::vector<Vertex> mesh;
        std::vector<unsigned> indices;

        unsigned vao_id;
        unsigned vbo_id;
        unsigned ebo_id;

        bool initialized = false;

        void upload_data();
    };
}