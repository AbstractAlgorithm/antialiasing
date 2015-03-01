#ifndef _CORE_MSAAT_H_
#define _CORE_MSAAT_H_

#include "AATechnique.h"
#include "rt.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: MSAATechnique (class for MSAA technique)
////////////////////////////////////////////////////////////////////////////////

class MSAATechnique : public AATechnique
{
	RenderTarget* m_multisampleRT;
	GLuint m_samples;
public:
    GLuint samples;

	MSAATechnique(GLuint s=4)
		: AATechnique()
		, m_samples(s)
        , samples(s)
	{
		glEnable(GL_MULTISAMPLE);
		m_multisampleRT = new RenderTarget(g_screenSizeV2.x / 3, g_screenSizeV2.y, true, m_samples);
	}

	void Draw(World* world)
	{
        GLuint prev = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&prev);
        RenderTarget::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_samples != samples)
        {
            delete m_multisampleRT;
            m_samples = samples;
            m_multisampleRT = new RenderTarget(g_screenSizeV2.x / 3, g_screenSizeV2.y, true, m_samples);
        }
		
		RenderTarget::set(m_multisampleRT);
		RenderTarget::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (world)
			world->Draw(0);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prev);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multisampleRT->fbo());
		glBlitFramebuffer(0, 0, g_screenSizeV2.x / 3, g_screenSizeV2.y, 0, 0, g_screenSizeV2.x / 3, g_screenSizeV2.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	~MSAATechnique()
	{
		delete m_multisampleRT;
	}
};

#endif