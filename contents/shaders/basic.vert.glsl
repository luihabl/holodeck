#version 330 core

layout (location = 0) in vec3 vert_pos;

out vec3 frag_pos;
// out vec2 frag_uv;

uniform mat4 vert_model; 
uniform mat4 vert_view; 
uniform mat4 vert_proj;

void main()
{
    gl_Position = vert_proj * vert_view * vert_model * vec4(vert_pos, 1.0f);
    frag_pos = vert_pos;
}

