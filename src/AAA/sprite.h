#ifndef _sprite_h_
#define _sprite_h_

#include "core.h"
#include "vec2.h"
#include "texture.h"
#include "shader.h"
#include "camera2d.h"

static const char* sprite_vs_source =
"#version 330 core" "\n"
"layout(location = 0) in vec2 aPosition;" "\n"
"uniform vec2 cameraT;" "\n"
"uniform vec2 size;" "\n"
"uniform vec2 pos;" "\n"
"uniform float angle;" "\n"
"out vec2 uv;" "\n"

"void main()" "\n"
"{" "\n"
    "uv = (aPosition+1.0)/2.0;" "\n"
    "vec2 rlp = aPosition*size;" "\n"
    "vec2 rpos = vec2(rlp.x*cos(angle)-rlp.y*sin(angle),rlp.y*cos(angle)+rlp.x*sin(angle));" "\n"
    "vec2 spos = rpos*cameraT;" "\n"
    "spos = spos + 2.0*pos*cameraT;" "\n"
    "gl_Position = vec4(spos, 0.5, 1.0);" "\n"
"}";

static const char* sprite_fs_source =
"#version 330 core" "\n"
"uniform sampler2DArray texSprite;" "\n"
"uniform int lejer;" "\n"
"in vec2 uv;" "\n"
"out vec4 fragColor;" "\n"

"void main()" "\n"
"{" "\n"
    "fragColor = texture(texSprite,vec3(uv,lejer));" "\n"
"}";

////////////////////////////////////////////////////////////////////////////////
// Class name: Sprite
////////////////////////////////////////////////////////////////////////////////

class Sprite
{
public:
    //Texture2D* m_texture;
    static TextureArray* m_texture;
    vec2 m_pos;
    float m_angle;
    vec2 m_scale;
    int m_layer;
	float m_rnd;

    Sprite()
        : m_pos(0.0, 0.0)
        , m_angle(0.0)
        , m_scale(1.0, 1.0)
        , m_layer(0)
		, m_rnd(1.0)
    {}

	void draw(Camera2D* camera, vec2 vr, ShaderProgram* overrideProgram = 0)
    {
        ShaderProgram* curr;
        curr = overrideProgram
                ? overrideProgram
                : program;
        curr->use();
        vec2 v = camera->GetTransform();
        glUniform2f(glGetUniformLocation(*curr, "cameraT"), v.x, v.y);
        glUniform2f(glGetUniformLocation(*curr, "resolution"), vr.x, vr.y);
        glUniform2f(glGetUniformLocation(*curr, "size"), m_scale.x, m_scale.y);
        glUniform2f(glGetUniformLocation(*curr, "pos"), m_pos.x, m_pos.y);
        glUniform1f(glGetUniformLocation(*curr, "angle"), m_angle + m_rnd);
        glUniform1i(glGetUniformLocation(*curr, "lejer"), m_layer);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture->texture());
        glUniform1i(tex_unif_loc, 0);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        //glFlush();
    }
    /*static void setD(Camera2D* camera, vec2 vr)
    {
        program->use();
        vec2 v = camera->GetTransform();
        glUniform2f(glGetUniformLocation(*program, "cameraT"), v.x, v.y);
		glUniform2f(glGetUniformLocation(*program, "resolution"), vr.x, vr.y);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_texture->texture());
        glUniform1i(tex_unif_loc, 0);
        glBindVertexArray(vao);
    }*/

	static void setPGAA(bool t)
	{
		glUniform1i(glGetUniformLocation(*program, "PGAA"), t ? 1 : 0);
	}

    static GLuint vao, vbo;
    static ShaderProgram* program;
	static Shader* vs_shader, *fs_shader, *gs_shader;
    static GLint tex_unif_loc;
    static void Init()
    {
        // init vao and vbo
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

        // init shaders
		vs_shader = new Shader(GL_VERTEX_SHADER);
		if (!vs_shader->source(sprite_vs_source))
			printf("%s\n", vs_shader->log());

		fs_shader = new Shader(GL_FRAGMENT_SHADER);
		if (!fs_shader->source(sprite_fs_source))
			printf("%s\n", fs_shader->log());

		(program = new ShaderProgram())->add(vs_shader)->add(fs_shader)->link();
		printf("%s\n", program->log());

        tex_unif_loc = glGetUniformLocation(*program, "texSprite");
    }

    static void Destroy()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        delete m_texture;

        delete vs_shader;
		//delete gs_shader;
        delete fs_shader;
        delete program;
    }
};

GLuint Sprite::vao = 0;
GLuint Sprite::vbo = 0;
ShaderProgram* Sprite::program = NULL;
Shader* Sprite::vs_shader = NULL;
Shader* Sprite::gs_shader = NULL;
Shader* Sprite::fs_shader = NULL;
GLint Sprite::tex_unif_loc = -1;
TextureArray* Sprite::m_texture = 0;

#endif