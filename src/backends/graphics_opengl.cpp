#include "../graphics.h"
#include "../shader.h"
#include "../texture.h"
#include "../model.h"
#include "../log.h"
#include "../file.h"

#include <string>
#include <cstring>
#include <glad/glad.h>
#include <glm/glm.hpp>

#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

constexpr const char* default_vertex_src = "#version 330 core\n\
layout (location = 0) in vec2 vertex_pos;\n\
layout (location = 1) in vec2 vertex_uv;\n\
layout (location = 2) in vec4 vertex_color;\n\
layout (location = 3) in vec3 vertex_cmix;\n\
out vec2 uv;\n\
out vec4 color;\n\
out vec3 cmix;\n\
uniform mat4 projection;\n\
void main()\n\
{\n\
    uv = vertex_uv;\n\
    color = vertex_color;\n\
    cmix = vertex_cmix;\n\
    gl_Position = projection * vec4(vertex_pos.xy, 0.0, 1.0);\n\
}";

constexpr const char* default_frag_src = "#version 330 core\n\
in vec2 uv;\n\
in vec4 color;\n\
in vec3 cmix;\n\
out vec4 output_color;\n\
uniform sampler2D image;\n\
void main()\n\
{\n\
    vec4 tex_color = texture(image, uv);\n\
    output_color = tex_color * color * cmix.x + tex_color.a * color * cmix.y + color * cmix.z;\n\
}";



using namespace holodeck;

#pragma region Graphics //====================================================================

void Graphics::load_gl_functions(GLLoaderFunction gl_loader)
{
    ASSERT_MSG(gladLoadGLLoader((GLADloadproc) gl_loader), "Failed to load GL functions");
}

void Graphics::clear(const glm::vec3 & c) {
    glClearColor(c.x, c.y, c.z, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void Graphics::clear(const glm::vec4 & c) {
    glClearColor(c.x, c.y, c.z, c.w); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

int Graphics::major_version() 
{
    return OPENGL_VERSION_MAJOR;
}

int Graphics::minor_version() 
{
    return OPENGL_VERSION_MINOR;
}

void Graphics::init()
{
    Log::info("Graphics: OpenGL %s %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));
    glEnable(GL_DEPTH_TEST);
}

void Graphics::viewport(int w, int h) {
    glViewport(0, 0, w, h);
}

void Graphics::viewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}

#pragma endregion

#pragma region Shader //======================================================================

Shader & Shader::use() {
    if(this->compiled) {
        glUseProgram(this->id);
    }        
    else {
        Log::warn("Trying to use empty shader.");
    }
        
    return *this;
}

void Shader::del() {
    glDeleteProgram(this->id);
    compiled = false;
}

unsigned int Shader::add(const char* src, unsigned int type) {

    unsigned int id_shader = glCreateShader( (GLenum) type);
    GLint src_len = (GLint) strlen(src);

    glShaderSource(id_shader, 1, &src, &src_len);
    glCompileShader(id_shader);

    int success;
    char err_msg[1024];
    glGetShaderiv(id_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id_shader, 1024, NULL, err_msg);
        Log::error("Error compiling shader: %s\n", err_msg);
    }
 
    glAttachShader(id, id_shader);
    return id_shader;
}

void Shader::compile_all(const char* vertex_src, const char* frag_src, const char*  geom_src) {
    
    unsigned int id_vertex, id_frag, id_geom=0;

    this->id = glCreateProgram();

    id_vertex = add(vertex_src, GL_VERTEX_SHADER);
    id_frag   = add(frag_src, GL_FRAGMENT_SHADER);
    if (geom_src != nullptr) 
        id_geom  = add(geom_src, GL_GEOMETRY_SHADER);
    
    glLinkProgram(this->id);

    int success;
    char err_msg[1024];
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->id, 1024, NULL, err_msg);
        Log::error("Error linking shaders: %s\n", err_msg);
    }

    glDeleteShader(id_vertex);
    glDeleteShader(id_frag);
    if (geom_src != nullptr) 
        glDeleteShader(id_geom);

    this->compiled = (bool) success;
}

Shader Shader::from_source(const char* vertex_src, const char* frag_src, const char*  geom_src) {
    Shader shader;
    shader.compile_all(vertex_src, frag_src, geom_src);
    return shader;
}

Shader Shader::from_file(const char* vertex_path, const char* frag_path, const char*  geom_path) {
    Shader shader;
    std::string vertex_src, frag_src, geom_src;

    vertex_src = File::load_txt(vertex_path);
    frag_src   = File::load_txt(frag_path);
    if(geom_path != nullptr)
        geom_src   = File::load_txt(geom_path);

    if (frag_src != "" && vertex_src != "")
        shader.compile_all(vertex_src.c_str(), frag_src.c_str(), geom_path != nullptr ? geom_src.c_str() : nullptr);
    return shader;    
}

Shader Shader::default_sprite_shaders() 
{
    return from_source(default_vertex_src, default_frag_src);
}

int Shader::find_uniform(const char *name)
{
    return glGetUniformLocation(this->id, name);
}

void Shader::set_float(const char * name, const float & value) const {
    glUniform1f(glGetUniformLocation(this->id, name), value);
}

void Shader::set_int(const char * name, const int & value) const {
    glUniform1i(glGetUniformLocation(this->id, name), value);
}

void Shader::set_mat4(const char * name, const glm::mat4 & mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, &(mat[0][0]));
}

void Shader::set_vec2(const char * name, const glm::vec2 & vec) const {
    glUniform2f(glGetUniformLocation(this->id, name), vec[0], vec[1]);
}

void Shader::set_vec3(const char * name, const glm::vec3 & vec) const {
    glUniform3f(glGetUniformLocation(this->id, name), vec[0], vec[1], vec[2]);
}

void Shader::set_vec4(const char * name, const glm::vec4 & vec) const {
    glUniform4f(glGetUniformLocation(this->id, name), vec[0], vec[1], vec[2], vec[3]);
}

// void Shader::set_dvec2(const char * name, const DVec2 & vec) const {
//     glUniform2d(glGetUniformLocation(this->id, name), vec[0], vec[1]);
// }

#pragma endregion

#pragma region Texture

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, this->id); 
}

void Texture::load(const Loader::DDSFile& dds)
{
	unsigned int components  = (dds.format == Loader::DDSFile::Compression::DXT1) ? 3 : 4; 
	unsigned int format;
	switch(dds.format) 
	{ 
	case Loader::DDSFile::Compression::DXT1: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
		break; 
	case Loader::DDSFile::Compression::DXT3: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
		break; 
	case Loader::DDSFile::Compression::DXT5: 
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
		break; 
	default: 
		return; 
	}

	// Create one OpenGL texture
	glGenTextures(1, &this->id);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, this->id);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	
	unsigned int block_size = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int offset = 0;

    unsigned int width = dds.header.dwWidth;
    unsigned int height = dds.header.dwHeight;

	/* load the mipmaps */ 
	for (unsigned int level = 0; level < dds.header.dwMipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*block_size; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, dds.data.data() + offset); 
	 
		offset += size; 
		width  /= 2; 
		height /= 2; 

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	} 
}

void Texture::load(const Loader::PNGFile& png)
{

    glGenTextures(1, &this->id);

    glBindTexture(GL_TEXTURE_2D, this->id);

    GLint format = png.n_comp == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, png.w, png.h, 0, format, GL_UNSIGNED_BYTE, png.data);

    // add a way to set these parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

#pragma endregion
