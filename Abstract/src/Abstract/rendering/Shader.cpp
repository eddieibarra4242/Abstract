#include "shader.h"
#include "../core/Debug.h"
#include <fstream>
#include <vector>

namespace Abstract {

    static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
    static std::string loadShader(const std::string& filename);
    static GLuint createShader(const std::string& text, GLenum type);

    Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile)
    {
        program = glCreateProgram();
        shaders[0] = createShader(loadShader(vertexFile), GL_VERTEX_SHADER);
        shaders[1] = createShader(loadShader(fragmentFile), GL_FRAGMENT_SHADER);

        for (unsigned int i = 0; i < NUM_SHADERS; i++)
        {
            glAttachShader(program, shaders[i]);
        }

        glLinkProgram(program);
        checkShaderError(program, GL_LINK_STATUS, true, "Program Linking Failed");

        glValidateProgram(program);
        checkShaderError(program, GL_VALIDATE_STATUS, true, "Program Invalid");
    }

    Shader::~Shader()
    {
        if (program == 0)
        {
            return;
        }

        for (unsigned int i = 0; i < NUM_SHADERS; i++)
        {
            glDetachShader(program, shaders[i]);
            glDeleteShader(shaders[i]);
        }

        glDeleteProgram(program);
    }

    Shader::Shader(Shader&& other) noexcept(true)
    {
        this->program = other.program;

        for (unsigned int i = 0; i < NUM_SHADERS; i++)
        {
            this->shaders[i] = other.shaders[i];
        }

        this->uniforms = other.uniforms;
        other.program = 0;
    }

    void Shader::operator=(Shader&& other) noexcept(true)
    {
        this->program = other.program;

        for (unsigned int i = 0; i < NUM_SHADERS; i++)
        {
            this->shaders[i] = other.shaders[i];
        }

        this->uniforms = other.uniforms;
        other.program = 0;
    }

    void Shader::bind()
    {
        glUseProgram(program);
    }

    void Shader::addUniform(const std::string& uniformName)
    {
        if (uniforms.count(uniformName) != 0)
        {
            return;
        }

        int uniformLocation = glGetUniformLocation(program, uniformName.c_str());

        if (uniformLocation == -1)
        {
            DEBUG_WARN("Shader", "Uniform %s is not being used or could not be found!", uniformName.c_str());
        }
        else
        {
            uniforms[uniformName] = uniformLocation;
        }
    }

    static std::string loadShader(const std::string& filename)
    {
        std::ifstream file;

        file.open(filename.c_str());

        std::string output;
        std::string line;

        if (file.is_open())
        {
            while (file.good())
            {
                std::getline(file, line);
                output.append(line + "\n");
            }
        }
        else
        {
            DEBUG_ERROR("Shader", "Shader %s could not be found!", filename.c_str());
        }

        return output;
    }

    static void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
    {
        GLint success = 0;
        GLchar error[1024] = { 0 };

        if (isProgram)
            glGetProgramiv(shader, flag, &success);
        else
            glGetShaderiv(shader, flag, &success);

        if (success == GL_FALSE)
        {
            if (isProgram)
                glGetProgramInfoLog(shader, sizeof(error), NULL, error);
            else
                glGetShaderInfoLog(shader, sizeof(error), NULL, error);

            DEBUG_ERROR("Shader", "%s:\n%s", errorMessage.c_str(), error);
        }
    }

    static GLuint createShader(const std::string& text, GLenum type)
    {
        GLuint shader = glCreateShader(type);

        if (shader == 0)
        {
            DEBUG_ERROR("Shader", "Error compiling shader type %s", (type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : "GL_FRAGMENT_SHADER"));
        }

        const GLchar* shaderSourceStrings[1];
        GLint shaderSourceStringLengths[1];

        shaderSourceStrings[0] = text.c_str();

        shaderSourceStringLengths[0] = text.length();

        glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
        glCompileShader(shader);

        checkShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

        return shader;
    }

}