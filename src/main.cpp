
#include "platform.h"
#include "graphics.h"
#include "shader.h"
#include "log.h"
#include "model.h"

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

    Shader basic_shader = Shader::from_file("contents/shaders/basic.vert.glsl", "contents/shaders/basic.frag.glsl");
    basic_shader.use();

    // ---- OpenGL stuff that will go away

    Model cube = Model::unit_cube();
    Model cube2 = cube;

    // Matrices
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(0, 4, -8));
    model2 = glm::rotate(model2, glm::radians(20.0f), glm::vec3(1, 0, 0));
    

    // glm::mat4 view = glm::mat4(1.0f);
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 proj = glm::mat4(1.0f);
    // proj = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
    // proj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 50.0f);


    basic_shader.set_mat4("proj", proj);

    float v = 0.2f;

    struct Camera
    {
        glm::vec3 pos = glm::vec3(0, 0, 3);
        glm::vec3 target = glm::vec3(0, 0, 0);
        glm::vec3 direction = glm::normalize(pos - target);
        glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), direction));
        glm::vec3 up = glm::cross(direction, right);
        glm::vec3 front = glm::vec3(0, 0, -1);

        float speed = 0.2f;


        float yaw = -90.0f;
        float pitch = 0.0f;

        void compute_direction(float delta_yaw, float delta_pitch)
        {

            yaw += delta_yaw;
            pitch += delta_pitch;

            if(pitch > 89.0f)
                pitch =  89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;

            float yawr = glm::radians(yaw);
            float pitchr = glm::radians(pitch);

            direction.x = cos(yawr) * cos(pitchr);
            direction.y = sin(pitchr);
            direction.z = sin(yawr) * cos(pitchr);

            front = glm::normalize(direction);
        }

    } camera;

    glm::mat4 view = glm::mat4(1.0f);

    platform.update();
    
    float mouse_sensitivity = 0.1f;
    platform.callbacks().on_mouse_movement = [&]()
    {
        camera.compute_direction(
            mouse_sensitivity * (float) platform.state.mouse.offset_x, 
            -mouse_sensitivity * (float) platform.state.mouse.offset_y
        );

        Log::info("[%d, %d]", platform.state.mouse.offset_x, platform.state.mouse.offset_y);
    };

    platform.callbacks().on_exit = [&quit]() { quit = true; };

    while(!quit)
    {

        proj = glm::perspective(glm::radians(45.0f), (float) platform.state.win.drawable_w / platform.state.win.drawable_h, 0.1f, 100.0f);
        basic_shader.set_mat4("proj", proj);

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