#version 330 core
out vec4 frag_color;

in vec4 frag_pos;

void main()
{
    frag_color = frag_pos;
}

