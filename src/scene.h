#pragma once 

#include "model.h"
#include "shader.h"
#include "camera.h"
#include "constants.h"

#include <vector>

namespace holodeck
{
    struct Scene
    {
        
        void add_model(const Model::Ref& model);

        void render();

        Camera& get_camera()
        {
            return camera;
        }

        void set_projection(const glm::mat4 proj)
        {
            projection_matrix = proj;
        }

        void set_light(const glm::vec4& _light_pos, const glm::vec4& _light_color)
        {
            light_color = _light_color;
            light_pos = _light_pos;
        }



        Camera camera;

        std::vector<Model::Ref> models;
        
        glm::mat4 projection_matrix;

        // make this more generic to allow more lights!
        glm::vec4 light_color = Color::white;
        glm::vec4 light_pos = glm::vec4(0.0f);
    };
}