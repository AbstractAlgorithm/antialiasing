#ifndef _CORE_CANVAS3PASS_H_
#define _CORE_CANVAS3PASS_H_

#include "postprocessing.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Canvas3Pass (display three images postprocessing pass)
////////////////////////////////////////////////////////////////////////////////

#define C3PASS_UNIF0 "colorScene1"
#define C3PASS_UNIF1 "colorScene2"
#define C3PASS_UNIF2 "colorScene3"

static const char* Canvas3PassPass_source =
"#version 330 core" "\n"
"in vec2 uv;" "\n"
"uniform sampler2D " C3PASS_UNIF0 ";" "\n"
"uniform sampler2D " C3PASS_UNIF1 ";" "\n"
"uniform sampler2D " C3PASS_UNIF2 ";" "\n"
"out vec4 fragColor;" "\n"
"void main()" "\n"
"{" "\n"
	"vec3 col = vec3(1.0);" "\n"
	"if (uv.x<=1.0/3.0) col = texture("C3PASS_UNIF0", vec2(uv.x*3.0, uv.y)).rgb;" "\n"
	"else if (uv.x<=2.0/3.0) col = texture("C3PASS_UNIF1", vec2(uv.x*3.0-1.0, uv.y)).rgb;" "\n"
	"else col = texture("C3PASS_UNIF2", vec2(uv.x*3.0-2.0, uv.y)).rgb;" "\n"
	"fragColor = vec4(col, 1.0);" "\n"
"}";

class Canvas3Pass : public PPEffect
{
	// uniforms
	RenderTarget *m_one, *m_two, *m_three;
	GLint m_onePos, m_twoPos, m_threePos;
public:

	Canvas3Pass(RenderTarget* one, RenderTarget* two, RenderTarget* three)
		: PPEffect(Canvas3PassPass_source, PPEFFECT_STR)
	{
		// set uniforms, if find their locations
		m_one		= one;
		m_onePos	= glGetUniformLocation(*program, C3PASS_UNIF0);
		m_two		= two;
		m_twoPos	= glGetUniformLocation(*program, C3PASS_UNIF1);
		m_three		= three;
		m_threePos	= glGetUniformLocation(*program, C3PASS_UNIF2);
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

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_three->texture());
		glUniform1i(m_threePos, 2);
	}
};

#endif