#pragma once 

#include "shader.h"
#include "loader.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

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

        void load(const std::vector<glm::vec3> & pos, const std::vector<glm::vec3> & normals, const std::vector<glm::vec2> & uv, const std::vector<unsigned> _indices);
        void load(const std::vector<glm::vec4> & pos, const std::vector<glm::vec3> & normals, const std::vector<glm::vec2> & uv, const std::vector<unsigned> _indices);

        template <typename LoaderType>
        void load(const LoaderType& loader)
        {
            load(loader.positions, loader.normals, loader.uv, loader.indices);
        }

        std::string name;
    
        void compute_transform(const glm::mat4& parent = glm::mat4(1.0f));

        // Model state. You can set either transform or each individual
        // state and then call compute_transform. Only transform is 
        // going to be used 
        glm::mat4 transform = glm::mat4(1.0f);
        glm::vec3 translation = glm::vec3(0.0f);
        glm::vec3 scale = glm::vec3(1.0f);
        glm::quat quaternion = glm::quat(glm::vec4(0.0f));

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