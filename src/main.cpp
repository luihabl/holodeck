
#include "platform.h"
#include "graphics.h"
#include "shader.h"
#include "log.h"
#include "model.h"

#include "constants.h"

#include <glm/gtc/matrix_transform.hpp> //remove this
#include <vector>

using namespace holodeck;

int main()
{
    bool quit = false;
    
    PlatformConfig config;
    config.w = 800;
    config.h = 600;
    config.name = "holodeck";
    config.on_exit = [&quit]() { quit = true; };

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
    

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
    // proj = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, 0.1f, 10.0f);

    // basic_shader.set_mat4("vert_model", model);
    basic_shader.set_mat4("view", view);
    basic_shader.set_mat4("proj", proj);

    while(!quit)
    {
        platform.update();
        
        Graphics::clear(glm::vec3(0, 0.0f, 0.1f));

        cube.transform = glm::rotate(model, glm::radians((float)platform.get_time_ms()) / 20.f, glm::vec3(1, 1, 0));
        cube.render(&basic_shader);

        cube2.transform = glm::rotate(model2, glm::radians((float)platform.get_time_ms()) / 10.f, glm::vec3(1, 1, 0));
        cube2.render(&basic_shader);

        if(platform.input.keyboard.just_released(Key::A))
            Log::info("%d (%d), %d (%d)", platform.input.mouse.x, platform.input.mouse.global_x, platform.input.mouse.y, platform.input.mouse.global_y);


        platform.swap_buffers();
        
    }

    platform.terminate();

    return 0;
}