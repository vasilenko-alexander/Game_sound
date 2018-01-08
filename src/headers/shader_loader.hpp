#pragma once

#include <GL/glew.h>
#include <string>

namespace ge
{
    class ShaderLoader
    {
        GLuint program = 0;

    public:
        ShaderLoader();
        void init_program(const std::string& vert_path,
                          const std::string& frag_path);
        void use();
        void delete_program();
        const GLuint& get_program_id();

    private:
        std::string get_shader_source(const std::string& path);
        GLuint compile_shader(const std::string& src, GLenum type);
    };
}
