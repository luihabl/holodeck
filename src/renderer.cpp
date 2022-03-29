#include "renderer.h"
#include "constants.h"

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <type_traits>

using namespace holodeck;



Renderer::~Renderer() 
{
    if (initialized) 
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(1, &vbo_id);
        glDeleteBuffers(1, &ebo_id);
    }
}

void Renderer::init()
{
    glGenVertexArrays(1, &vao_id);
    
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    // ---- Adding vertex attributes here ----

    //glm::vec3 Vertex.pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));

    //Vec2 Vertex.uv
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));

    //Color Vertex.color
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*) offsetof(Vertex, color));

    //Vec<uint8_t, 3> Vertex.cmix
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*) offsetof(Vertex, cmix));
    
    // --------

    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::clear() 
{

    vertices.clear();
    indices.clear();
    
    transform_stack.clear();
    transform = Constant::identity<glm::mat4>;
    
    batch_stack.clear();
    batch = Batch();

}