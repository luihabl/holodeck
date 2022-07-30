#include "app.h"
#include "shader.h"
#include "texture.h"
#include "loader.h"
#include "model.h"
#include "camera.h"
#include "graphics.h"
#include "texture.h"
#include "constants.h"

using namespace holodeck;

App::App(const PlatformConfig& config)
{
    m_platform.init(config);
    m_gui.initialize(m_platform);
}

void App::run()
{
    m_quit = false;

    m_platform.relative_mouse();
    m_gui.use_mouse(false);

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

    Texture tex;
    tex.load(Loader::DDSFile("contents/meshes/suzanne.DDS"));

    // Model cube = Model::unit_cube();
    Model light_source = Model::unit_cube();

    glm::vec3 light_pos = glm::vec3(1.2f, 1.0f, 2.0f);
    light_source.translation = light_pos;
    light_source.scale = glm::vec3(0.2f);
    // light_source.quaternion = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(45.0f)));
    light_source.compute_transform();

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    cube_shader.use().set_mat4("proj", proj);
    source_shader.use().set_mat4("proj", proj);

    glm::vec4 light_color = Color::white;

    cube_shader.use().set_vec4("light_pos", glm::vec4(light_pos, 1.0f));
    cube_shader.use().set_vec4("light_color", light_color);
    cube_shader.use().set_vec4("object_color", glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
    source_shader.use().set_vec4("color", light_color);

    Camera camera;

    bool should_move = true;
    m_platform.callbacks().on_mouse_movement = [&]() {
        if(!should_move)
            return;

        float mouse_sensitivity = 0.1f;
        camera.compute_direction(
            mouse_sensitivity * (float) m_platform.state.mouse.offset_x, 
            -mouse_sensitivity * (float) m_platform.state.mouse.offset_y
        );
    };

    m_platform.callbacks().on_mouse_click = [&](int button, bool pressed) 
    {        
        if(button == 0 && pressed && !m_gui.wants_mouse())
        {
            m_platform.relative_mouse(true);
            should_move = true;
            m_gui.use_mouse(false);
        }
    };

    m_platform.callbacks().on_exit = [&]() { m_quit = true; };

    while(!m_quit)
    {
        m_platform.update();
                
        Graphics::clear(Color::black);

        if(!m_gui.wants_keyboard())
        {
            if(m_platform.state.keyboard.pressed(Key::A))
            {
                camera.pos -= camera.speed * glm::normalize(glm::cross(camera.front, camera.up));
            }
            if(m_platform.state.keyboard.pressed(Key::D))
            {
                camera.pos += camera.speed * glm::normalize(glm::cross(camera.front, camera.up));
            }
            if(m_platform.state.keyboard.pressed(Key::S))
            {
                camera.pos -= camera.speed * camera.front;
            }
            if(m_platform.state.keyboard.pressed(Key::W))
            {
                camera.pos += camera.speed * camera.front;
            }
            if(m_platform.state.keyboard.pressed(Key::Escape))
            {
                m_platform.relative_mouse(false);
                should_move = false;
                m_gui.use_mouse(true);
            }
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

        m_gui.render();

        m_platform.swap_buffers();
    }

    m_gui.shutdown();
    m_platform.terminate();
}