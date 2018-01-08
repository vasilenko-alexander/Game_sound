#include "headers/shader_loader.hpp"
#include <sstream>
#include <fstream>
#include <iostream>

namespace ge
{
    ShaderLoader::ShaderLoader()
    {
    }

    std::string ShaderLoader::get_shader_source(const std::string& path)
    {
        std::stringstream shader_src;
        std::ifstream file(path);
        if (!file.is_open())
        {
            std::cerr << "Can't open shader file" << std::endl;
            return shader_src.str();
        }

        shader_src << file.rdbuf();

        return shader_src.str();
    }

    GLuint ShaderLoader::compile_shader(const std::string& src, GLenum type)
    {
        GLuint shader_id     = glCreateShader(type);
        const GLchar* source = src.data();
        glShaderSource(shader_id, 1, &source, nullptr);
        glCompileShader(shader_id);

        GLint compile_result = 0;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result);
        if (compile_result == GL_FALSE)
        {
            GLint log_size = 0;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_size);
            // log size include index for null-term char
            char* log_info = new char[log_size];
            glGetShaderInfoLog(shader_id, log_size, &log_size, log_info);
            std::cerr << log_info << std::endl;
            delete[] log_info;
            glDeleteShader(shader_id);
            return 0;
        }

        return shader_id;
    }

    void ShaderLoader::init_program(const std::string& vert_path,
                                    const std::string& frag_path)
    {
        std::string vertex_src = get_shader_source(vert_path);
        GLuint vs              = compile_shader(vertex_src, GL_VERTEX_SHADER);

        if (vs == 0)
            return;

        std::string frag_src = get_shader_source(frag_path);
        GLuint fs            = compile_shader(frag_src, GL_FRAGMENT_SHADER);

        if (fs == 0)
            return;

        program = glCreateProgram();

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glBindAttribLocation(program, 1, "coords");
        glBindAttribLocation(program, 2, "tex_coords");

        glLinkProgram(program);

        GLint link_result = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &link_result);
        if (link_result == GL_FALSE)
        {
            GLint log_size = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);

            char* log_info = new char[log_size];
            glGetProgramInfoLog(program, log_size, &log_size, log_info);
            std::cerr << log_info << std::endl;

            delete[] log_info;
            glDeleteProgram(program);

            glDeleteShader(vs);
            glDeleteShader(fs);
            return;
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    void ShaderLoader::use()
    {
        glUseProgram(program);
    }

    const GLuint& ShaderLoader::get_program_id()
    {
        return program;
    }

    void ShaderLoader::delete_program()
    {
        glDeleteProgram(program);
    }
}
