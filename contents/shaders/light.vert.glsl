#version 330 core

layout (location = 0) in vec4 vert_pos;
layout (location = 1) in vec3 vert_normal;
layout (location = 2) in vec2 vert_uv;

out vec4 frag_pos;
out vec2 frag_uv;
out vec3 frag_normal;

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vert_pos;

    frag_pos = model * vert_pos;
    frag_normal = mat3(transpose(inverse(model))) * vert_normal;
    frag_uv = vert_uv;
}

