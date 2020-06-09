#include "Texture.h"
#include "../core/Debug.h"
#include "Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cassert>

namespace Abstract {

    Texture::Texture(const std::string& filename)
    {
        int numComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);

        if (data == NULL)
        {
            DEBUG_ERROR("Texture", "Failed To Load File: %s", filename.c_str());
            return;
        }

        loadTexture(width, height, GL_RGBA, GL_RGBA, GL_REPEAT, GL_LINEAR, data);

        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            DEBUG_ERROR("Framebuffer", "Framebuffer creation Failed: %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        }

        stbi_image_free(data);
    }

    Texture::Texture(int width, int height, GLint internalFormat, GLint format, GLint wrapMode, GLfloat filter, GLubyte* data)
    {
        this->width = width;
        this->height = height;

        loadTexture(width, height, internalFormat, format, wrapMode, filter, data);

        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    }

    Texture::~Texture()
    {
        if (textureID != 0 && frameBuffer != 0)
        {
            glDeleteTextures(1, &textureID);
            glDeleteFramebuffers(1, &frameBuffer);
        }
    }

    Texture::Texture(Texture& other)
    {
        this->width = other.width;
        this->height = other.height;

        loadTexture(other.width, other.height, GL_RGBA, GL_RGBA, GL_REPEAT, GL_LINEAR, NULL);

        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            DEBUG_ERROR("Framebuffer", "Framebuffer creation Failed: %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        }

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, other.frameBuffer);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBlitFramebuffer(0, 0, other.width, other.height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    void Texture::operator=(Texture& other)
    {
        if (textureID == 0)
        {
            this->width = other.width;
            this->height = other.height;

            loadTexture(other.width, other.height, GL_RGBA, GL_RGBA, GL_REPEAT, GL_LINEAR, NULL);

            glGenFramebuffers(1, &frameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                DEBUG_ERROR("Framebuffer", "Framebuffer creation Failed: %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
            }
        }

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, other.frameBuffer);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBlitFramebuffer(0, 0, other.width, other.height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    Texture::Texture(Texture&& other) noexcept(true)
    {
        this->textureID = other.textureID;
        this->frameBuffer = other.frameBuffer;
        this->width = other.width;
        this->height = other.height;
        other.textureID = 0;
        other.frameBuffer = 0;
    }

    void Texture::operator=(Texture&& other) noexcept(true)
    {
        this->textureID = other.textureID;
        this->frameBuffer = other.frameBuffer;
        this->width = other.width;
        this->height = other.height;
        other.textureID = 0;
        other.frameBuffer = 0;
    }

    void Texture::loadTexture(int width, int height, GLint internalFormat, GLint format, GLint wrapMode, GLfloat filter, GLubyte* data)
    {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }

    void Texture::bind(unsigned int unit) const
    {
        assert(unit >= 0 && unit <= 31);
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    void Texture::bindAsRenderTarget() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glViewport(0, 0, width, height);
    }

    void Texture::resolveFrom(const FrameBufferObject& src, GLenum readMode)
    {
        src.resolveTo(frameBuffer, width, height, GL_COLOR_ATTACHMENT0, readMode);
    }

    FrameBufferObject::FrameBufferObject(uint32_t width, uint32_t height, size_t numAttachments, GLuint* interalformats, GLuint* formats, GLenum* attachments) : renderBuffers()
    {
        this->width = width;
        this->height = height;
        frameBuffer = 0;
        initRenderTargets(interalformats, attachments, numAttachments);
    }

    FrameBufferObject::~FrameBufferObject()
    {
        if (frameBuffer != NULL)
        {
            for (std::vector<std::tuple<GLuint, GLenum, GLuint>>::iterator it = renderBuffers.begin(); it != renderBuffers.end(); ++it)
            {
                glDeleteRenderbuffers(1, &(std::get<2>(*it)));
            }

            glDeleteFramebuffers(1, &frameBuffer);
        }
    }

    FrameBufferObject::FrameBufferObject(FrameBufferObject& other)
    {
        this->width = other.width;
        this->height = other.height;

        GLuint* internalFormats = (GLuint*)malloc(other.renderBuffers.size() * sizeof(GLuint));
        GLenum* attachments = (GLenum*)malloc(other.renderBuffers.size() * sizeof(GLenum));

        for (uint32_t i = 0; i < other.renderBuffers.size(); i++)
        {
            internalFormats[i] = std::get<0>(other.renderBuffers[i]);
            attachments[i] = std::get<1>(other.renderBuffers[i]);
        }

        frameBuffer = 0;
        initRenderTargets(internalFormats, attachments, other.renderBuffers.size());

        free(internalFormats);
        free(attachments);

        for (std::vector<std::tuple<GLuint, GLenum, GLuint>>::iterator it = renderBuffers.begin(); it != renderBuffers.end(); ++it)
        {
            resolveFrom(other, std::get<1>(*it), std::get<1>(*it));
        }
    }

    void FrameBufferObject::operator=(FrameBufferObject& other)
    {
        if (frameBuffer != 0)
        {
            if (frameBuffer != NULL)
            {
                for (std::vector<std::tuple<GLuint, GLenum, GLuint>>::iterator it = renderBuffers.begin(); it != renderBuffers.end(); ++it)
                {
                    glDeleteRenderbuffers(1, &(std::get<2>(*it)));
                }

                glDeleteFramebuffers(1, &frameBuffer);
            }
        }

        this->width = other.width;
        this->height = other.height;

        GLuint* internalFormats = (GLuint*)malloc(other.renderBuffers.size() * sizeof(GLuint));
        GLenum* attachments = (GLenum*)malloc(other.renderBuffers.size() * sizeof(GLenum));

        for (uint32_t i = 0; i < other.renderBuffers.size(); i++)
        {
            internalFormats[i] = std::get<0>(other.renderBuffers[i]);
            attachments[i] = std::get<1>(other.renderBuffers[i]);
        }

        frameBuffer = 0;
        initRenderTargets(internalFormats, attachments, other.renderBuffers.size());

        free(internalFormats);
        free(attachments);

        for (std::vector<std::tuple<GLuint, GLenum, GLuint>>::iterator it = renderBuffers.begin(); it != renderBuffers.end(); ++it)
        {
            resolveFrom(other, std::get<1>(*it), std::get<1>(*it));
        }
    }

    FrameBufferObject::FrameBufferObject(FrameBufferObject&& other) noexcept(true)
    {
        this->frameBuffer = other.frameBuffer;
        this->width = other.width;
        this->height = other.height;
        this->renderBuffers = other.renderBuffers;

        other.frameBuffer = NULL;
    }

    void FrameBufferObject::operator=(FrameBufferObject&& other) noexcept(true)
    {
        this->frameBuffer = other.frameBuffer;
        this->width = other.width;
        this->height = other.height;
        this->renderBuffers = other.renderBuffers;

        other.frameBuffer = NULL;
    }

    void FrameBufferObject::bindAsRenderTarget() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glViewport(0, 0, width, height);
    }

    void FrameBufferObject::resolveFrom(const FrameBufferObject& src, GLenum drawMode, GLenum readMode)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, src.frameBuffer);
        glDrawBuffer(drawMode);
        glReadBuffer(readMode);
        glBlitFramebuffer(0, 0, src.width, src.height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    void FrameBufferObject::resolveTo(const GLuint& dest, int width, int height, GLenum drawMode, GLenum readMode) const
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
        glDrawBuffer(drawMode);
        glReadBuffer(readMode);
        glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    void FrameBufferObject::resolveToWindow(GLenum readMode) const
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
        glDrawBuffer(GL_BACK);
        glReadBuffer(readMode);
        glBlitFramebuffer(0, 0, width, height, 0, 0, Window::getWidth(), Window::getHeight(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    void FrameBufferObject::initRenderTargets(GLuint* interalformats, GLenum* attachments, size_t numAttachments)
    {
        if (attachments == NULL)
        {
            return;
        }

        GLenum drawBuffers[32] = { GL_NONE };
        bool hasDepth = false;

        for (uint32_t i = 0; i < numAttachments; i++)
        {
            if (attachments[i] == GL_DEPTH_ATTACHMENT)
            {
                drawBuffers[i] = GL_NONE;
                hasDepth = true;
            }
            else
            {
                drawBuffers[i] = attachments[i];
            }

            if (frameBuffer == 0)
            {
                glGenFramebuffers(1, &frameBuffer);
                glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
            }

            createRenderBuffer(interalformats[i], attachments[i]);
        }

        if (frameBuffer == 0)
        {
            return;
        }

        if (!hasDepth)
        {
            createRenderBuffer(GL_DEPTH_COMPONENT32, GL_DEPTH_ATTACHMENT);
        }

        glDrawBuffers(numAttachments, drawBuffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            DEBUG_ERROR("Framebuffer", "Framebuffer creation Failed: %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        }
    }

    void FrameBufferObject::createRenderBuffer(GLenum renderBufferStorage, GLenum attachment)
    {
        GLuint rb;
        glGenRenderbuffers(1, &rb);
        glBindRenderbuffer(GL_RENDERBUFFER, rb);
        glRenderbufferStorage(GL_RENDERBUFFER, renderBufferStorage, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rb);
        renderBuffers.push_back(std::make_tuple(renderBufferStorage, attachment, rb));
    }

}