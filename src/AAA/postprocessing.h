#ifndef _CORE_POSTPROCESSING_H_
#define _CORE_POSTPROCESSING_H_

#include "core.h"
#include "vec2.h"
#include "texture.h"
#include "shader.h"
#include "rt.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: PPEffect (postprocessing effect)
////////////////////////////////////////////////////////////////////////////////

#define PPEFFECT_STR 0
#define PPEFFECT_FILE 1

static const char* pp_vs_source =
"#version 330 core" "\n"
"layout(location = 0) in vec2 aPosition;" "\n"
"out vec2 uv;" "\n"

"void main()" "\n"
"{" "\n"
	"uv = (aPosition+1.0)/2.0;" "\n"
	"gl_Position = vec4(aPosition, 0.5, 1.0);" "\n"
"}";

class PPEffect {
protected:
	GLuint vao, vbo;
	ShaderProgram* program;
	Shader* vs_shdr, *fs_shdr;

	virtual void BindUniforms() = 0;
public:

	PPEffect(const char* src, char mode)
	{
		// fullscreen quad
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		GLfloat vertices[8] =
		{
			-1.0f, 1.0f,
			-1.0f, -1.0f,
			1.0f, 1.0f,
			1.0f, -1.0f
		};
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		// vertex shader
		vs_shdr = new Shader(GL_VERTEX_SHADER);
		if (!vs_shdr->source(pp_vs_source))
			printf("%s\n", vs_shdr->log());

		// fragment shader
		fs_shdr = new Shader(GL_FRAGMENT_SHADER);
		switch (mode)
		{
			case PPEFFECT_FILE:
				if (!fs_shdr->sourceFile(src))
					printf("%s\n", fs_shdr->log());
				break;
			case PPEFFECT_STR:
				if (!fs_shdr->source(src))
					printf("%s\n", fs_shdr->log());
				break;
		}

		// program
		program = new ShaderProgram();
		program->add(vs_shdr)->add(fs_shdr);
		if(program->link())
			printf("%s\n", program->log());
	}
	void Draw()
	{
		program->use();
		BindUniforms();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	~PPEffect()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
		delete vs_shdr;
		delete fs_shdr;
		delete program;
	}
};

#endif