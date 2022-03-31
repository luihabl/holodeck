#include "model.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace holodeck;

void Model::init()
{
    if (initialized)
        return;

    glGenVertexArrays(1, &vao_id);
    
    glBindVertexArray(vao_id);

    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

    // ---- Adding vertex attributes here ----

    //glm::vec4 Vertex.pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));

    //glm::vec3 Vertex.normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

    //glm::vec2 Vertex.uv
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));

    // --------

    glGenBuffers(1, &ebo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    initialized = true;
}

void Model::clear()
{
    if (!initialized)
        return;

    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
    glDeleteBuffers(1, &ebo_id);

    mesh.clear();
    indices.clear();
}

void Model::render(Shader* shader)
{
    if(shader)
        shader->use().set_mat4("model", transform);

    glBindVertexArray(vao_id);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Model::load(const std::vector<glm::vec3> & pos, const std::vector<unsigned> _indices)
{
    if(!initialized)
        return;

    indices = _indices;
    for (const auto& v : pos)
        mesh.push_back({
            glm::vec4(v, 1.0f),
            glm::vec3(0.0f),
            glm::vec2(0.0f)
        });

    upload_data();
}

void Model::load(const std::vector<glm::vec4> & pos, const std::vector<unsigned> _indices)
{
    if(!initialized)
        return;

    indices = _indices;
    for (const auto& v : pos)
        mesh.push_back({
            v,
            glm::vec3(0.0f),
            glm::vec2(0.0f)
        });

    upload_data();
}


void Model::upload_data()
{
    glBindVertexArray(vao_id);

    // Uploading vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.size(), mesh.data(), GL_STATIC_DRAW);

    // Uploading indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Model::compute_transform(const glm::mat4& parent)
{
    transform = parent;
    transform = glm::translate(transform, translation);
    transform = transform * glm::toMat4(quaternion);
    transform = glm::scale(transform, scale);
}

Model Model::unit_cube()
{
    std::vector<glm::vec3> vertices = {
        {-0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        {-0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f}
    };

    std::vector<unsigned> indices = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        7, 3, 0,
        0, 4, 7,
        6, 2, 1,
        1, 5, 6,
        0, 1, 5,
        5, 4, 0,
        3, 2, 6,
        6, 7, 3,
    };

    Model cube;
    cube.init();
    cube.load(vertices, indices);

    return cube;
}