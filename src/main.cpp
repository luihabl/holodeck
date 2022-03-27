
#include "platform.h"
#include "graphics.h"
#include "shader.h"

#include <glm/gtc/matrix_transform.hpp> //remove this
#include <glad/glad.h> //remove this

using namespace holodeck;

struct Vertex
{
    glm::vec4 pos;
};

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

    // Initializing OpenGL

    Vertex vertices[] = {
        {{0.5f, 0.5f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f, 1.0f}}
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int vao_id;
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    unsigned int vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //This becomes dynamic and goes into the loop

    //Vec4 Vertex.pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));

    unsigned int ebo_id;
    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //This becomes dynamic and goes into the loop

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Matrices
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1, 0, 0));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

    basic_shader.set_mat4("vert_model", model);
    basic_shader.set_mat4("vert_view", view);
    basic_shader.set_mat4("vert_proj", proj);

    while(!quit)
    {
        platform.update();
        
        Graphics::clear(glm::vec3(0, 0.1f, 0.1f));

        basic_shader.set_mat4("vert_model", glm::rotate(model, glm::radians((float)platform.get_time_ms()) / 20.f, glm::vec3(1, 0, 0)));

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        platform.swap_buffers();
        
    }

    platform.terminate();

    return 0;
}