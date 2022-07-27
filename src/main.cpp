
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
        "contents/shaders/const.vert.glsl", 
        "contents/shaders/const.frag.glsl"
    );

    Model cube;
    cube.load(Loader::OBJFile("contents/meshes/teapot.obj", true));

    //Model lines = Model::grid(10, 10);
    // lines.translation = glm::vec3(0.0f, 0.0f, -25.0f);
    
    // lines.compute_transform();
    // float grid_scale = 7.0f;
    // lines.scale = glm::vec3(2.0f * grid_scale);

    // float grid_offset_x = 0.0f;
    // float grid_offset_y = 3.2f;
    // float grid_offset_z = -0.2f;

    // std::vector<glm::vec3> grid_translations = 
    // {
    //     {grid_offset_x, grid_offset_y, grid_offset_z - grid_scale},
    //     {grid_offset_x, grid_offset_y, grid_offset_z + grid_scale},
    //     {grid_offset_x, grid_offset_y - grid_scale, grid_offset_z},
    //     {grid_offset_x, grid_offset_y + grid_scale, grid_offset_z},   
    //     {grid_offset_x - grid_scale, grid_offset_y, grid_offset_z},
    //     {grid_offset_x + grid_scale, grid_offset_y, grid_offset_z}
    // };
    // std::vector<glm::vec3> grid_rotations = 
    // {
    //     glm::vec3(0.0f),
    //     glm::vec3(0.0f),
    //     glm::vec3(glm::radians(90.0f), 0.0f, 0.0f),
    //     glm::vec3(glm::radians(90.0f), 0.0f, 0.0f),
    //     glm::vec3(0.0f, glm::radians(90.0f), 0.0f),
    //     glm::vec3(0.0f, glm::radians(90.0f), 0.0f)
    // };
    // lines.mode = Model::DrawMode::Lines;


    Texture tex;
    tex.load(Loader::DDSFile("contents/meshes/suzanne.DDS"));

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
    source_shader.use().set_vec4("color", light_color);

    Camera camera;

    bool should_move = true;
    platform.callbacks().on_mouse_movement = [&]() {
        if(!should_move)
            return;

        float mouse_sensitivity = 0.1f;
        camera.compute_direction(
            mouse_sensitivity * (float) platform.state.mouse.offset_x, 
            -mouse_sensitivity * (float) platform.state.mouse.offset_y
        );
    };

    platform.callbacks().on_mouse_click = [&](int button, bool pressed) {
        if(button == 0 && pressed)
        {
            platform.relative_mouse(true);
            should_move = true;
        }
    };

    platform.callbacks().on_exit = [&quit]() { quit = true; };

    while(!quit)
    {
        platform.update();
                
        Graphics::clear(Color::black);

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
        if(platform.state.keyboard.pressed(Key::Escape))
        {
            platform.relative_mouse(false);
            should_move = false;
        }

        cube_shader.use().set_mat4("view", glm::lookAt(camera.pos, camera.pos + camera.front, camera.up));
        cube_shader.use().set_vec3("view_pos", camera.pos);
        // cube.transform = glm::rotate(glm::mat4(1.0f), glm::radians((float)platform.get_time_ms()) / 20.f, glm::vec3(1, 1, 0));

        tex.bind();
        cube_shader.use().set_int("tex_sampler", 0);
        cube.render(&cube_shader);

        source_shader.use().set_mat4("view", glm::lookAt(camera.pos, camera.pos + camera.front, camera.up));
        source_shader.set_vec4("color", light_color);
        light_source.render(&source_shader);
        
        source_shader.set_vec4("color", Color::yellow);

        // for (int i = 0; i < 6; i++)
        // {
        //     lines.quaternion = glm::quat(grid_rotations[i]);
        //     lines.translation = grid_translations[i];
        //     lines.compute_transform();

        //     lines.render(&source_shader);
        // }

        platform.swap_buffers();
    }

    platform.terminate();

    return 0;
}