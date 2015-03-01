#ifndef _CORE_DIFFPASS_H_
#define _CORE_DIFFPASS_H_

#include "postprocessing.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: DiffPass (difference postprocessing pass)
////////////////////////////////////////////////////////////////////////////////

#define DIFFPASS_UNIF0 "colorScene1"
#define DIFFPASS_UNIF1 "colorScene2"

static const char* DiffPass_source =
"#version 330 core" "\n"
"in vec2 uv;" "\n"
"uniform sampler2D " DIFFPASS_UNIF0 ";" "\n"
"uniform sampler2D " DIFFPASS_UNIF1 ";" "\n"
"out vec4 fragColor;" "\n"
"void main()" "\n"
"{" "\n"
	"vec3 a = texture("DIFFPASS_UNIF0", uv).rgb;" "\n"
	"vec3 b = texture("DIFFPASS_UNIF1", uv).rgb;" "\n"
	"fragColor = vec4(abs(a-b), 1.0);" "\n"
"}";

class DiffPass : public PPEffect
{
	// uniforms
	RenderTarget *m_one, *m_two;
	GLint m_onePos, m_twoPos;
public:

	DiffPass(RenderTarget* one, RenderTarget* two)
		: PPEffect(DiffPass_source, PPEFFECT_STR)
	{
		// set uniforms, if find their locations
		m_one = one;
		m_onePos = glGetUniformLocation(*program, DIFFPASS_UNIF0);
		m_two = two;
		m_twoPos = glGetUniformLocation(*program, DIFFPASS_UNIF1);
	}

	void BindUniforms()
	{
		// bind uniforms
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_one->texture());
		glUniform1i(m_onePos, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_two->texture());
		glUniform1i(m_twoPos, 1);
	}
};

#endif