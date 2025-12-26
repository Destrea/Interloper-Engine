#pragma once

#include "Renderer.h"
#include <glad/glad.h>
namespace Renderer
{
    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;
    };

    class Framebuffer
    {
    public:

        //TODO Fix issues with the framebuffer

        Framebuffer(const FramebufferSpecification& specification = FramebufferSpecification());
        ~Framebuffer() { Shutdown(); }




        void Init();
        void Invalidate();
        void Bind();
        void Unbind();
        void Rescale(uint32_t width, uint32_t height);
        void Shutdown();
        FramebufferSpecification GetSpec() {return m_Specification; }

        GLuint GetColorAttachmentRendererID() const { return m_ColorAttachment; }
        GLuint GetFBO();
        GLuint GetRBO();
        GLuint GetFTex();

    private:
        GLuint m_FramebufferID = 0;
        GLuint m_ColorAttachment = 0;
        GLuint m_DepthAttachment = 0;

        FramebufferSpecification m_Specification;



    };

}
