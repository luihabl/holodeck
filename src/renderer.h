#pragma once 

#include "shader.h"

#include <glm/glm.hpp>
#include <vector>

namespace holodeck
{
    class Renderer
    {

    public:
        Renderer() = default;
        ~Renderer();

        void init();


    private:

        struct Vertex
        {
            glm::vec4 pos;
            // glm::vec2 uv;
            // glm::vec3 normal;
        };

        struct Batch
        {
            unsigned count = 0;
            unsigned offset = 0;
            Shader* shader = nullptr;
        };

        glm::mat4 transform;
        std::vector<glm::mat4> transform_stack;

        Batch batch;
        std::vector<Batch> batch_stack;

        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;

        bool initialized = false;
        unsigned vao_id;
        unsigned vbo_id;
        unsigned ebo_id;

        void clear();

    };
}