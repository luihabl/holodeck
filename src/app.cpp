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

    Shader::Ref single_light_shader = Shader::from_file(
        "contents/shaders/light.vert.glsl", 
        "contents/shaders/light.frag.glsl"
    );

    m_scene.set_projection(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f));


    Model::Ref cube = Model::from_loader(Loader::OBJFile("contents/meshes/suzanne.obj", true));
    Model::Ref light_source = Model::unit_cube();

    Texture::Ref tex = Texture::from_dds(Loader::DDSFile("contents/meshes/suzanne.DDS"));

    glm::vec4 light_color = Color::white;
    glm::vec4 light_pos = glm::vec4(1.2f, 1.0f, 2.0f, 1.0f);
    light_source->translation = light_pos;
    light_source->scale = glm::vec3(0.2f);
    light_source->compute_transform();
    m_scene.set_light(light_pos, light_color);

    cube->color_mul = Color::white;
    cube->color_add = Color::black;
    cube->tex = tex;

    light_source->color_mul = Color::black;
    light_source->color_const = light_color;

    cube->shader = single_light_shader;
    light_source->shader = single_light_shader;

    m_scene.add_model(cube);
    m_scene.add_model(light_source);

    bool should_move = true;
    m_platform.callbacks().on_mouse_movement = [&]() {
        if(!should_move)
            return;

        float mouse_sensitivity = 0.1f;
        m_scene.get_camera().compute_direction(
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
                m_scene.get_camera().pos -= m_scene.get_camera().speed * glm::normalize(glm::cross(m_scene.get_camera().front, m_scene.get_camera().up));
            }
            if(m_platform.state.keyboard.pressed(Key::D))
            {
                m_scene.get_camera().pos += m_scene.get_camera().speed * glm::normalize(glm::cross(m_scene.get_camera().front, m_scene.get_camera().up));
            }
            if(m_platform.state.keyboard.pressed(Key::S))
            {
                m_scene.get_camera().pos -= m_scene.get_camera().speed * m_scene.get_camera().front;
            }
            if(m_platform.state.keyboard.pressed(Key::W))
            {
                m_scene.get_camera().pos += m_scene.get_camera().speed * m_scene.get_camera().front;
            }
            if(m_platform.state.keyboard.pressed(Key::Escape))
            {
                m_platform.relative_mouse(false);
                should_move = false;
                m_gui.use_mouse(true);
            }
        }

        m_scene.render();
        m_gui.render(&m_scene);
        m_platform.swap_buffers();
    }

    m_gui.shutdown();
    m_platform.terminate();
}