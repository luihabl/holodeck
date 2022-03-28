#version 330 core
out vec4 frag_color;

in vec3 frag_pos;

void main()
{
    frag_color = vec4(frag_pos.x*frag_pos.x, frag_pos.y*frag_pos.y, frag_pos.z*frag_pos.z, 1) + 0.5;
}

