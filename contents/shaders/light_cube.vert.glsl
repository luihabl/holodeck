#version 330 core

layout (location = 0) in vec4 vert_pos;
layout (location = 1) in vec3 vert_normal;
layout (location = 2) in vec2 vert_uv;

// out vec2 frag_uv;

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vert_pos;
}

