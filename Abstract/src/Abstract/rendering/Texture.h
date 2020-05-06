#pragma once

#include <string>
#include <GL/glew.h>
#include <vector>
#include <tuple>
#include "../core/Platform.h"

namespace Abstract {

    class ABSTRACT_API FrameBufferObject;

    class ABSTRACT_API Texture
    {
    public:
        Texture() : textureID(0), frameBuffer(0), width(0), height(0) { }
        Texture(const std::string& filename);
        Texture(int width, int height, GLint internalFormat, GLint format, GLint wrapMode, GLfloat filter, GLubyte* data);

        inline int getWidth() const { return width; }
        inline int getHeight() const { return height; }

        void bind(unsigned int unit) const;
        void bindAsRenderTarget() const;

        void resolveFrom(const FrameBufferObject& src, GLenum readMode);

        virtual ~Texture();

        Texture(Texture& other);
        void operator=(Texture& other);

        Texture(Texture&& other) noexcept(true);
        void operator=(Texture&& other) noexcept(true);
    private:
        GLuint textureID;
        GLuint frameBuffer;

        int width;
        int height;

        void loadTexture(int width, int height, GLint internalFormat, GLint format, GLint wrapMode, GLfloat filter, GLubyte* data);
    };

    class ABSTRACT_API FrameBufferObject
    {
    public:
        FrameBufferObject() : frameBuffer(NULL), width(0), height(0), renderBuffers() { }
        FrameBufferObject(uint32_t width, uint32_t height, size_t numAttachments, GLuint* interalformats, GLuint* formats, GLenum* attachments);

        void bindAsRenderTarget() const;
        void resolveFrom(const FrameBufferObject& src, GLenum drawMode, GLenum readMode);
        void resolveTo(const GLuint& dest, int width, int height, GLenum drawMode, GLenum readMode) const;
        void resolveToWindow(GLenum readMode) const;

        inline uint32_t getWidth() const
        {
            return width;
        }

        inline uint32_t getHeight() const
        {
            return height;
        }

        FrameBufferObject(FrameBufferObject& other);
        void operator=(FrameBufferObject& other);

        FrameBufferObject(FrameBufferObject&& other) noexcept(true);
        void operator=(FrameBufferObject&& other) noexcept(true);

        ~FrameBufferObject();
    private:
        GLuint frameBuffer;

        uint32_t width;
        uint32_t height;

        std::vector<std::tuple<GLuint, GLenum, GLuint>> renderBuffers;

        void initRenderTargets(GLuint* interalformats, GLenum* attachments, size_t numAttachments);
        void createRenderBuffer(GLenum renderBufferStorage, GLenum attachment);
    };

}