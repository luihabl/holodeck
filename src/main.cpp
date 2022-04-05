
#include "platform.h"
#include "graphics.h"
#include "shader.h"
#include "log.h"
#include "model.h"
#include "camera.h"
#include "loader.h"
#include "texture.h"

#include "constants.h"

#include <glm/gtc/matrix_transform.hpp> //remove this
#include <cmath>

using namespace holodeck;

int main()
{
    bool quit = false;
    
    PlatformConfig config;
    config.w = 800;
    config.h = 600;
    config.name = "holodeck";

    Platform platform;
    platform.init(config);
    platform.relative_mouse();

    Shader cube_shader = Shader::from_file(
        "contents/shaders/light.vert.glsl", 
        "contents/shaders/light.frag.glsl"
    );

    Shader source_shader = Shader::from_file(
        "contents/shaders/light_source.vert.glsl", 
        "contents/shaders/light_source.frag.glsl"
    );

    Model cube;
    cube.load(Loader::OBJFile("contents/meshes/suzanne.obj", true));

    Texture tex;
    tex.load(Loader::PNGFile("contents/meshes/sample-bumblebee-400x300.png"));

    // Model cube = Model::unit_cube();
    Model light_source = Model::unit_cube();

    // ---- Matrices ----

    glm::vec3 light_pos = glm::vec3(1.2f, 1.0f, 2.0f);

    light_source.translation = light_pos;
    light_source.scale = glm::vec3(0.2f);
    // light_source.quaternion = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(45.0f)));
    light_source.compute_transform();


    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float) 800/ (float)600, 0.1f, 100.0f);
    cube_shader.use().set_mat4("proj", proj);
    source_shader.use().set_mat4("proj", proj);

    glm::vec4 light_color = Color::white;

    cube_shader.use().set_vec4("light_pos", glm::vec4(light_pos, 1.0f));
    cube_shader.use().set_vec4("light_color", light_color);
    cube_shader.use().set_vec4("object_color", glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
    source_shader.use().set_vec4("light_color", light_color);

    Camera camera;
    
    platform.callbacks().on_mouse_movement = [&]() {
        float mouse_sensitivity = 0.1f;
        camera.compute_direction(
            mouse_sensitivity * (float) platform.state.mouse.offset_x, 
            -mouse_sensitivity * (float) platform.state.mouse.offset_y
        );
    };
    platform.callbacks().on_exit = [&quit]() { quit = true; };

    while(!quit)
    {
        platform.update();
                
        Graphics::clear(glm::vec3(0, 0.0f, 0.1f));

        if(platform.state.keyboard.pressed(Key::A))
        {
            camera.pos -= camera.speed * glm::normalize(glm::cross(camera.front, camera.up));
        }
        if(platform.state.keyboard.pressed(Key::D))
        {
            camera.pos += camera.speed * glm::normalize(glm::cross(camera.front, camera.up));
        }
        if(platform.state.keyboard.pressed(Key::S))
        {
            camera.pos -= camera.speed * camera.front;
        }
        if(platform.state.keyboard.pressed(Key::W))
        {
            camera.pos += camera.speed * camera.front;
        }

        cube_shader.use().set_mat4("view", glm::lookAt(camera.pos, camera.pos + camera.front, camera.up));
        cube_shader.use().set_vec3("view_pos", camera.pos);
        // cube.transform = glm::rotate(glm::mat4(1.0f), glm::radians((float)platform.get_time_ms()) / 20.f, glm::vec3(1, 1, 0));

        tex.bind();
        cube_shader.use().set_int("tex_sampler", 0);
        
        cube.render(&cube_shader);

        source_shader.use().set_mat4("view", glm::lookAt(camera.pos, camera.pos + camera.front, camera.up));
        light_source.render(&source_shader);

        platform.swap_buffers();
    }

    platform.terminate();

    return 0;
}