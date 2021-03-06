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

void Model::render()
{
    if(shader)
        shader->use().set_mat4("model", transform);

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(vao_id);

    GLenum draw_mode;
    switch (mode)
    {
        case DrawMode::Triangles:
            draw_mode = GL_TRIANGLES;
            break;

        case DrawMode::Lines:
            draw_mode = GL_LINES;
            break;

        default:
            draw_mode = GL_TRIANGLES;
    }

	glDrawElements(draw_mode, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Model::load(const std::vector<glm::vec3> & pos, const std::vector<glm::vec3> & normals, const std::vector<glm::vec2> & uv, const std::vector<unsigned> _indices)
{
    init();

    indices = _indices;
    for (int i = 0; i < pos.size(); i++)
    {

        glm::vec3 position(0.0f);
        if(i < pos.size()) position = pos[i];

        glm::vec3 normal(0.0f);
        if(i < normals.size()) normal = normals[i];

        glm::vec2 tex_pos(0.0f);
        if(i < uv.size()) tex_pos = uv[i];

        mesh.push_back({
            glm::vec4(position, 1.0f),
            normal,
            tex_pos
            });
    }

    upload_data();
}

void Model::load(const std::vector<glm::vec4> & pos, const std::vector<glm::vec3> & normals, const std::vector<glm::vec2> & uv, const std::vector<unsigned> _indices)
{
    init();
    
    indices = _indices;
    for (int i = 0; i < pos.size(); i++)
    {
        glm::vec4 position(0.0f);
        if(i < pos.size()) position = pos[i];

        glm::vec3 normal(0.0f);
        if(i < normals.size()) normal = normals[i];

        glm::vec2 tex_pos(0.0f);
        if(i < uv.size()) tex_pos = uv[i];

        mesh.push_back({
            position,
            normal,
            tex_pos
        });
    }

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

Model::Ref Model::unit_cube()
{
    std::vector<glm::vec3> vertices = {
        {-0.5f,  -0.5f,  -0.5f},
        {0.5f,  -0.5f,   -0.5f},
        { 0.5f,   0.5f,  -0.5f},
        {-0.5f,   0.5f,  -0.5f},
        {-0.5f,  -0.5f,   0.5f},
        { 0.5f,  -0.5f,   0.5f},
        { 0.5f,   0.5f,   0.5f},
        {-0.5f,   0.5f,   0.5f},
        {-0.5f,   0.5f,   0.5f},
        {-0.5f,   0.5f,  -0.5f},
        {-0.5f,  -0.5f,  -0.5f},
        {-0.5f,  -0.5f,   0.5f},
        { 0.5f,   0.5f,   0.5f},
        { 0.5f,   0.5f,  -0.5f},
        { 0.5f,  -0.5f,  -0.5f},
        { 0.5f,  -0.5f,   0.5f},
        {-0.5f,  -0.5f,  -0.5f},
        { 0.5f,  -0.5f,  -0.5f},
        { 0.5f,  -0.5f,   0.5f},
        {-0.5f,  -0.5f,   0.5f},
        {-0.5f,   0.5f,  -0.5f},
        { 0.5f,   0.5f,  -0.5f},
        { 0.5f,   0.5f,   0.5f},
        {-0.5f,   0.5f,   0.5f}
    };

    std::vector<glm::vec3> normals = {
        {  0.0f,   0.0f,  -1.0f},
        {  0.0f,   0.0f,  -1.0f},
        {  0.0f,   0.0f,  -1.0f},
        {  0.0f,   0.0f,  -1.0f},
        {  0.0f,   0.0f,  1.0f},
        {  0.0f,   0.0f,  1.0f},
        {  0.0f,   0.0f,  1.0f},
        {  0.0f,   0.0f,  1.0f},
        { -1.0f,   0.0f,   0.0f},
        { -1.0f,   0.0f,   0.0f},
        { -1.0f,   0.0f,   0.0f},
        { -1.0f,   0.0f,   0.0f},
        {  1.0f,   0.0f,   0.0f},
        {  1.0f,   0.0f,   0.0f},
        {  1.0f,   0.0f,   0.0f},
        {  1.0f,   0.0f,   0.0f},
        {  0.0f,  -1.0f,   0.0f},
        {  0.0f,  -1.0f,   0.0f},
        {  0.0f,  -1.0f,   0.0f},
        {  0.0f,  -1.0f,   0.0f},
        {  0.0f,   1.0f,   0.0f},
        {  0.0f,   1.0f,   0.0f},
        {  0.0f,   1.0f,   0.0f},
        {  0.0f,   1.0f,   0.0f}
    };

    std::vector<unsigned> indices = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        8, 9, 10,
        10, 11, 8,
        12, 13, 14,
        14, 15, 12,
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20
    };

    Model::Ref cube = std::make_shared<Model>();
    cube->load(vertices, normals, {}, indices);

    return cube;
}

Model::Ref Model::grid(int nx, int ny, float l_x, float l_y)
{
    float dx = l_x / ((float) nx - 1.0f); 
    float dy = l_y / ((float) ny - 1.0f); 

    std::vector<glm::vec3> vertices;
    std::vector<unsigned> indices;

    for(int i = 0; i < nx; i++)
    {
        vertices.push_back({-l_x / 2.0f, (float) i * dy - l_y / 2.0f, 0.0f});
        indices.push_back(vertices.size() - 1);
        vertices.push_back({l_x / 2.0f, (float) i * dy - l_y / 2.0f, 0.0f});
        indices.push_back(vertices.size() - 1);
    }

    for(int i = 0; i < ny; i++)
    {
        vertices.push_back({(float) i * dx - l_x / 2.0f, -l_y / 2.0f, 0.0f});
        indices.push_back(vertices.size() - 1);
        vertices.push_back({(float) i * dx - l_x / 2.0f, l_y / 2.0f, 0.0f});
        indices.push_back(vertices.size() - 1);
    }

    Model::Ref grid = std::make_shared<Model>();
    grid->load(vertices, {}, {}, indices);

    return grid;
}