#ifndef _CORE_SSAAT_H_
#define _CORE_SSAAT_H_

#include "AATechnique.h"
#include "rt.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SSAATechnique (class for SSAA technique)
////////////////////////////////////////////////////////////////////////////////

#define SSAA_UNIF_NAME "colorTex"

static const char* SSAAPass_source =
"#version 330 core" "\n"
"in vec2 uv;" "\n"
"uniform sampler2D " SSAA_UNIF_NAME ";" "\n"
"out vec4 fragColor;" "\n"
"void main()" "\n"
"{" "\n"
    "fragColor = texture("SSAA_UNIF_NAME", uv);" "\n"
"}";

static class SSAAPass : public PPEffect
{
    // uniforms
    RenderTarget* m_colorRT;
    GLint m_colorPos;
public:

    SSAAPass(RenderTarget* t)
        : PPEffect(SSAAPass_source, PPEFFECT_STR)
    {
        m_colorRT = t;
        m_colorPos = glGetUniformLocation(*program, SSAA_UNIF_NAME);
    }

    void BindUniforms()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_colorRT->texture());
        glUniform1i(m_colorPos, 0);
    }
};

class SSAATechnique : public AATechnique
{
    RenderTarget* m_bigRT;
    uint16_t m_size;
    SSAAPass* m_ssaa;
public:
    uint16_t size;

    SSAATechnique(uint16_t s = 2)
        : AATechnique()
        , m_size(s)
        , size(s)
    {
        m_bigRT = new RenderTarget(m_size * g_screenSizeV2.x / 3, m_size * g_screenSizeV2.y);
        glBindTexture(GL_TEXTURE_2D, m_bigRT->texture());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        m_ssaa = new SSAAPass(m_bigRT);
    }

    void Draw(World* world)
    {
        GLuint prev = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&prev);

        if (m_size != size)
        {
            delete m_bigRT;
            m_size = size;
            m_bigRT = new RenderTarget(m_size * g_screenSizeV2.x / 3, m_size * g_screenSizeV2.y);
            glBindTexture(GL_TEXTURE_2D, m_bigRT->texture());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }

        RenderTarget::set(m_bigRT);                                             // render to big RT
        RenderTarget::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (world)
            world->Draw(0);

        //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prev);
        //glBindFramebuffer(GL_READ_FRAMEBUFFER, m_bigRT->fbo());
        //glBlitFramebuffer(0, 0, m_size * g_screenSizeV2.x / 3, m_size * g_screenSizeV2.y, 0, 0, g_screenSizeV2.x / 3, g_screenSizeV2.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, m_bigRT->texture());
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, prev);                                // run postprocessing pass
        glViewport(0, 0, g_screenSizeV2.x / 3, g_screenSizeV2.y);
        RenderTarget::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_ssaa->Draw();
    }

    ~SSAATechnique()
    {
        delete m_bigRT;
        delete m_ssaa;
    }
};

#endif