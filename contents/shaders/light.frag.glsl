#version 330 core
out vec4 frag_color;

in vec4 frag_pos;
in vec3 frag_normal;

uniform vec4 light_pos;
uniform vec4 light_color;
uniform vec4 object_color;
uniform vec3 view_pos;

void main()
{
    // Reflection factors
    float ambient_factor = 0.1f;
    float diffuse_factor = 1.0f;
    float specular_factor = 0.5f;
    float shininess = 32;

    // Vectors
    vec3 normal = normalize(frag_normal);
    vec3 light_dir = normalize(light_pos.xyz - frag_pos.xyz);
    vec3 view_dir = normalize(view_pos - frag_pos.xyz);
    vec3 reflect_dir = reflect(-light_dir, normal);

    // Ambient light
    vec4 ambient_light = ambient_factor * light_color;

    // Diffuse light
    vec4 diffuse_light = diffuse_factor * light_color * max(dot(normal, light_dir), 0.0f);

    // Specular light
    vec4 specular_light = specular_factor * light_color * pow(max(dot(view_dir, reflect_dir), 0.0f), shininess);

    // Final color
    frag_color = (ambient_light + diffuse_light + specular_light) * object_color;
}

