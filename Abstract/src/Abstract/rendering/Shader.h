#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>
#include <GL/glew.h>

#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../core/Platform.h"

namespace Abstract {

    class ABSTRACT_API Shader
    {
    public:
        Shader() : program(0) { }
        Shader(const std::string& filename) : Shader(filename + ".vert", filename + ".frag") {}
        Shader(const std::string& vertexFile, const std::string& fragmentFile);

        void bind();

        inline void sendTextureUnit(const std::string& name, const int& data)
        {
            addUniform(name);
            glUniform1i(uniforms[name], data);
        }

        inline void sendFloat(const std::string& name, const float& data)
        {
            addUniform(name);
            glUniform1f(uniforms[name], data);
        }

        inline void sendVector3(const std::string& name, const Vector3& data)
        {
            addUniform(name);
            glUniform3f(uniforms[name], data.getX(), data.getY(), data.getZ());
        }

        inline void sendMatrix4(const std::string& name, const Matrix4& data)
        {
            addUniform(name);
            glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, data.getData());
        }

        virtual ~Shader();

        Shader(Shader&& other) noexcept(true);
        void operator=(Shader&& other) noexcept(true);

    private:
        static const unsigned int NUM_SHADERS = 2;
        //Shader(const Shader &other) {}
        //void operator=(const Shader &other) {}

        std::map<std::string, GLuint> uniforms;

        GLuint program;
        GLuint shaders[NUM_SHADERS];

        void addUniform(const std::string& uniformName);
    };

}

#endif