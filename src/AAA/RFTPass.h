#ifndef _CORE_RFTPASS_H_
#define _CORE_RFTPASS_H_

#include "postprocessing.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: RFTPass (render from texture postprocessing pass)
////////////////////////////////////////////////////////////////////////////////

#define RFTPASS_UNIF_NAME "colorTex"

static const char* RFTPass_source =
"#version 330 core" "\n"
"in vec2 uv;" "\n"
"uniform sampler2D " RFTPASS_UNIF_NAME ";" "\n"
"out vec4 fragColor;" "\n"
"void main()" "\n"
"{" "\n"
"fragColor = texture(" RFTPASS_UNIF_NAME ", uv);" "\n"
"}";

class RFTPass : public PPEffect
{
	// uniforms
	RenderTarget* m_colorRT;
	GLint m_colorPos;
public:

	RFTPass(RenderTarget* t)
		: PPEffect(RFTPass_source, PPEFFECT_STR)
	{
		m_colorRT = t;
		m_colorPos = glGetUniformLocation(*program, RFTPASS_UNIF_NAME);
	}

	void BindUniforms()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colorRT->texture());
		glUniform1i(m_colorPos, 0);
	}
};

#endif