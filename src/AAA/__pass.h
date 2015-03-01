/*

#ifndef _CORE_####PASS_H_
#define _CORE_####PASS_H_

#include "postprocessing.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ####Pass (#### postprocessing pass)
////////////////////////////////////////////////////////////////////////////////


static const char* ####Pass_source =
"#version 330 core" "\n"
"in vec2 uv;" "\n"
"out vec4 fragColor;" "\n"
"void main()" "\n"
"{" "\n"
"fragColor = vec4(1.0);" "\n"
"}";

class ####Pass : public PPEffect
{
	// uniforms
	
public:

	####Pass($$)
		: PPEffect(####Pass_source, PPEFFECT_STR)
	{
		// set uniforms, if find their locations
	}

	void BindUniforms()
	{
		// bind uniforms
	}
};

#endif

*/