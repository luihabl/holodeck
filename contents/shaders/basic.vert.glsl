#version 330 core

layout (location = 0) in vec4 vert_pos;

out vec4 frag_pos;
// out vec2 frag_uv;

uniform mat4 vert_model; 
uniform mat4 vert_view; 
uniform mat4 vert_proj;

void main()
{
    gl_Position = vert_proj * vert_view * vert_model * vert_pos;
    // gl_Position = vert_pos;
    frag_pos = gl_Position;
}

