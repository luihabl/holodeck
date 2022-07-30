#include "scene.h"
#include "shader.h"
#include "texture.h"

using namespace holodeck;

void Scene::add_model(const Model::Ref& model)
{
    models.push_back(model);
}

void Scene::render()
{
    Shader* last_shader = nullptr;
    Texture* last_tex = nullptr;

    for(const auto& model : models)
    {
        Shader* shader = model->shader.get();

        if(shader != last_shader)
        {
            shader->use().set_vec4("light_pos", light_pos);
            shader->set_vec4("light_color", light_color);
            shader->set_mat4("proj", projection_matrix);

            shader->set_mat4("view", glm::lookAt(camera.pos, camera.pos + camera.front, camera.up));
            shader->set_vec3("view_pos", camera.pos);
        }

        shader->set_vec4("object_color_mul", model->color_mul);
        shader->set_vec4("object_color_add", model->color_add);
        shader->set_vec4("object_color_const", model->color_const);

        Texture* tex = model->tex.get();

        if(tex != last_tex)
        {
            shader->set_int("tex_sampler", tex->tex_slot);
            tex->bind();
        }

        last_shader = shader;

        model->render();
    }
}