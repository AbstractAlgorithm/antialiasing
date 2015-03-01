#ifndef _CORE_PGAAT_H_
#define _CORE_PGAAT_H_

#include "AATechnique.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: PGAATechnique (class for PGAA technique)
////////////////////////////////////////////////////////////////////////////////

#define CONSERVATIVE_RASTERIZATION_NV 0x9346

class PGAATechnique : public AATechnique
{
    Shader
        *vs_shader,
        *gs_shader,
        *fs_shader;
    ShaderProgram* m_program;
    
public:
	PGAATechnique()
	{
        // init shaders
        vs_shader = new Shader(GL_VERTEX_SHADER);
        if (!vs_shader->sourceFile(RES_FOLDER"/pgaa_shader.vert"))
                  printf("%s\n", vs_shader->log());

        gs_shader = new Shader(GL_GEOMETRY_SHADER);
        if (!gs_shader->sourceFile(RES_FOLDER"/pgaa_shader.geom"))
        	printf("%s\n", gs_shader->log());

        fs_shader = new Shader(GL_FRAGMENT_SHADER);
        if (!fs_shader->sourceFile(RES_FOLDER"/pgaa_shader.frag"))
                  printf("%s\n", fs_shader->log());

        (m_program = new ShaderProgram())->add(vs_shader)->add(gs_shader)->add(fs_shader)->link();
              printf("%s\n", m_program->log());
	}

	void Draw(World* world)
	{
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(CONSERVATIVE_RASTERIZATION_NV);

        RenderTarget::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (world)
            world->Draw(m_program);

        //glDisable(CONSERVATIVE_RASTERIZATION_NV);
        glDisable(GL_BLEND);
	}

	~PGAATechnique()
	{
        delete vs_shader;
        delete gs_shader;
        delete fs_shader;
        delete m_program;
	}
};

#endif