#ifndef _CORE_INVERTPASS_H_
#define _CORE_INVERTPASS_H_

#include "postprocessing.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: InvertPass (invert colors postprocessing pass)
////////////////////////////////////////////////////////////////////////////////

#define INVERTPASS_UNIF_NAME "colorTex"

static const char* InvertPass_source =
"#version 330 core" "\n"
"in vec2 uv;" "\n"
"uniform sampler2D " INVERTPASS_UNIF_NAME ";" "\n"
"out vec4 fragColor;" "\n"
"void main()" "\n"
"{" "\n"
	"fragColor = vec4( vec3(1.0) - texture(" INVERTPASS_UNIF_NAME ", uv).rgb, 1.0);" "\n"
"}";

class InvertPass : public PPEffect
{
	// uniforms
	RenderTarget* m_colorRT;
	GLint m_colorPos;

public:

	InvertPass(RenderTarget* t)
		: PPEffect(InvertPass_source, PPEFFECT_STR)
	{
		m_colorRT = t;
		m_colorPos = glGetUniformLocation(*program, INVERTPASS_UNIF_NAME);
	}

	void BindUniforms()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colorRT->texture());
		glUniform1i(m_colorPos, 0);
	}
};

#endif