#version 330 core
out vec4 frag_color;

// in vec3 frag_pos;

uniform vec4 light_color;
uniform vec4 object_color;

void main()
{
    frag_color = light_color * object_color;
}

