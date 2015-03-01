#ifndef _CORE_RT_H_
#define _CORE_RT_H_

#include "core.h"
#include "vec2.h"
#include "texture.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: RenderTarget
////////////////////////////////////////////////////////////////////////////////
class RenderTarget
{
private:
	GLuint m_fb;
	GLuint m_texture;
	GLuint m_rb;
	uint16_t m_width, m_height;
    bool m_multisample;
public:
	RenderTarget(uint16_t w, uint16_t h, bool ms = false, GLuint samples = 4);
	~RenderTarget();

	operator GLuint() const { return m_fb; }

	bool isReady() const;
	GLuint texture() const;
	GLuint fbo() const;

	static void clearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) { glClearColor(r, g, b, a); }
	static void clear(GLbitfield mask) { glClear(mask); }
	static void set(RenderTarget* rt = 0);
	static uint16_t BackbufferWidth, BackbufferHeight;
};

RenderTarget::RenderTarget(uint16_t w, uint16_t h, bool ms, GLuint samples ) : m_width(w), m_height(h)
{
    m_multisample = ms;
    if (!m_multisample)
	{
		// create texture
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		// create render buffer
		glGenRenderbuffersEXT(1, &m_rb);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rb);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_width, m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// create fbo
		glGenFramebuffersEXT(1, &m_fb);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fb);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rb);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		// create color buffer
		glEnable(GL_MULTISAMPLE);
		glGenRenderbuffers(1, &m_texture);
		glBindRenderbuffer(GL_RENDERBUFFER, m_texture);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGB8, m_width, m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// create render buffer
		glGenRenderbuffersEXT(1, &m_rb);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rb);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, m_width, m_height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// create fbo
		glGenFramebuffersEXT(1, &m_fb);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fb);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_texture);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rb);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

RenderTarget::~RenderTarget()
{
    GLint prev = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&prev);
    if (prev == m_fb)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    m_multisample
        ? glDeleteRenderbuffers(1, &m_texture)
        : glDeleteTextures(1, &m_texture);
	glDeleteRenderbuffers(1, &m_rb);
	glDeleteFramebuffers(1, &m_fb);
}

bool RenderTarget::isReady() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fb);
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	return status == GL_FRAMEBUFFER_COMPLETE;
}

GLuint RenderTarget::texture() const
{
	return m_texture;
}

GLuint RenderTarget::fbo() const
{
	return m_fb;
}

void RenderTarget::set(RenderTarget* rt)
{
	glViewport(0, 0, rt ? rt->m_width : BackbufferWidth, rt ? rt->m_height : BackbufferHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, rt ? rt->m_fb : 0);
}

uint16_t RenderTarget::BackbufferWidth	= 1;
uint16_t RenderTarget::BackbufferHeight = 1;

#endif