#ifndef _CORE_FXAAT_H_
#define _CORE_FXAAT_H_

#include "AATechnique.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: FXAATechnique (class for FXAA technique)
////////////////////////////////////////////////////////////////////////////////

#define FXAA_UNIF_NAME "texScene"

static const char* FXAAPass_source =
"#version 330 core" "\n"
"" "\n"
"in vec2 uv;" "\n"
"uniform sampler2D " FXAA_UNIF_NAME ";" "\n"
"out vec4 FragColor;" "\n"
"" "\n"
"#define FXAA_REDUCE_MIN (1.0 / 128.0) " "\n"
"#define FXAA_REDUCE_MUL (1.0 / 8.0) " "\n"
"#define FXAA_SPAN_MAX 8.0" "\n"
"" "\n"
"void main() " "\n"
"{" "\n"
"    vec2 resolution = vec2(1./600.0,1./600.0);" "\n"
"" "\n"
"    vec3 rgbNW = texture(texScene, (gl_FragCoord.xy + vec2(-1.0, -1.0)) * resolution).xyz;" "\n"
"    vec3 rgbNE = texture(texScene, (gl_FragCoord.xy + vec2(1.0, -1.0)) * resolution).xyz;" "\n"
"    vec3 rgbSW = texture(texScene, (gl_FragCoord.xy + vec2(-1.0, 1.0)) * resolution).xyz;" "\n"
"    vec3 rgbSE = texture(texScene, (gl_FragCoord.xy + vec2(1.0, 1.0)) * resolution).xyz;" "\n"
"    vec3 rgbM = texture(texScene, gl_FragCoord.xy * resolution).xyz;" "\n"
"    vec3 luma = vec3(0.299, 0.587, 0.114);" "\n"
"" "\n"
"    float lumaNW = dot(rgbNW, luma);" "\n"
"    float lumaNE = dot(rgbNE, luma);" "\n"
"    float lumaSW = dot(rgbSW, luma);" "\n"
"    float lumaSE = dot(rgbSE, luma);" "\n"
"    float lumaM = dot(rgbM, luma);" "\n"
"    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));" "\n"
"    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));" "\n"
"" "\n"
"    vec2 dir;" "\n"
"    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));" "\n"
"    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));" "\n"
"" "\n"
"    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);" "\n"
"    " "\n"
"    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);" "\n"
"    " "\n"
"    dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * rcpDirMin)) * resolution;" "\n"
"    " "\n"
"    vec3 rgbA = 0.5 * ( " "\n"
"        texture(texScene, gl_FragCoord.xy * resolution + dir * (1.0 / 3.0 - 0.5)).xyz " "\n"
"        + texture(texScene, gl_FragCoord.xy * resolution + dir * (2.0 / 3.0 - 0.5)).xyz);" "\n"
"" "\n"
"    vec3 rgbB = rgbA * 0.5 + 0.25 * (" "\n"
"        texture(texScene, gl_FragCoord.xy * resolution + dir * -0.5).xyz " "\n"
"        + texture(texScene, gl_FragCoord.xy * resolution + dir * 0.5).xyz);" "\n"
"" "\n"
"    float lumaB = dot(rgbB, luma);" "\n"
"" "\n"
"    if ((lumaB < lumaMin) || (lumaB > lumaMax)) {" "\n"
"        FragColor = vec4(rgbA, 1.0);" "\n"
"    } else {" "\n"
"        FragColor = vec4(rgbB, 1.0);" "\n"
"    }" "\n"
"}";



static class FXAAPass : public PPEffect
{
    // uniforms
    RenderTarget* m_colorRT;
    GLint m_colorPos;
public:

    FXAAPass(RenderTarget* t)
        : PPEffect(FXAAPass_source, PPEFFECT_STR)
    {
        m_colorRT = t;
        m_colorPos = glGetUniformLocation(*program, FXAA_UNIF_NAME);
    }

    void BindUniforms()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_colorRT->texture());
        glUniform1i(m_colorPos, 0);
    }
};

class FXAATechnique : public AATechnique
{
    RenderTarget* m_colorRT;
    FXAAPass* m_fxaaPass;
public:
	FXAATechnique()
	{
        m_colorRT = new RenderTarget(g_screenSizeV2.x / 3, g_screenSizeV2.y);
        m_fxaaPass = new FXAAPass(m_colorRT);
	}

	void Draw(World* world)
	{
        GLint prev = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&prev);

        RenderTarget::set(m_colorRT);                                           // render-to-texture
        RenderTarget::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (world)
			world->Draw(0);

        glBindFramebuffer(GL_FRAMEBUFFER, prev);                                // run postprocessing pass
        RenderTarget::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_fxaaPass->Draw();
	}

	~FXAATechnique()
	{
        delete m_colorRT;
        delete m_fxaaPass;
	}
};

#endif