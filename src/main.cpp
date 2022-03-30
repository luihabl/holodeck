
#include "platform.h"
#include "graphics.h"
#include "shader.h"
#include "log.h"
#include "model.h"
#include "camera.h"

#include "constants.h"

#include <glm/gtc/matrix_transform.hpp> //remove this
#include <vector>
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

    Shader basic_shader = Shader::from_file(
        "contents/shaders/light_cube.vert.glsl", 
        "contents/shaders/light_cube.frag.glsl"
    );
    basic_shader.use();

    Model cube = Model::unit_cube();
    Model cube2 = cube;

    // ---- Matrices ----
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(0, 4, -8));
    model2 = glm::rotate(model2, glm::radians(20.0f), glm::vec3(1, 0, 0));

    glm::mat4 view = glm::mat4(1.0f);
    
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float) 800/600, 0.1f, 100.0f);
    basic_shader.set_mat4("proj", proj);

    basic_shader.set_vec4("light_color", Color::white);

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

        view  = glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
        basic_shader.set_mat4("view", view);

        cube.transform = glm::rotate(model, glm::radians((float)platform.get_time_ms()) / 20.f, glm::vec3(1, 1, 0));
        cube.render(&basic_shader);

        cube2.transform = glm::rotate(model2, glm::radians((float)platform.get_time_ms()) / 10.f, glm::vec3(1, 1, 0));
        cube2.render(&basic_shader);

        platform.swap_buffers();
    }

    platform.terminate();

    return 0;
}