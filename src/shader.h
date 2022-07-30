#pragma once 

#include <glm/glm.hpp>
#include <memory>

namespace holodeck {
    class Shader {
        public:
            using Ref = std::shared_ptr<Shader>;

            Shader() = default;
            
            unsigned int id;
            bool compiled = false;
            
            Shader & use();
            void del();

            int find_uniform(const char *name);
            void set_float(const char * name, const float & value) const;
            void set_int(const char * name, const int & value) const;
            void set_mat4(const char * name, const glm::mat4 & mat) const;
            void set_vec4(const char * name, const glm::vec4 & vec) const;
            void set_vec3(const char * name, const glm::vec3 & vec) const;
            void set_vec2(const char * name, const glm::vec2 & vec) const;
            // add other uniform types

            static Ref from_source(const char* vertex_src, const char* frag_src, const char*  geom_src = nullptr);
            static Ref from_file(const char* vertex_path, const char* frag_path, const char*  geom_path = nullptr);
            static Ref default_sprite_shaders();

        private: 
            unsigned int add(const char* source, unsigned int type);
            void compile_all(const char* vertex_src, const char* frag_src, const char*  geom_src = nullptr);
    };
}