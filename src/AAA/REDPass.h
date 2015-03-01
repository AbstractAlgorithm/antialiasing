#ifndef _CORE_REDPASS_H_
#define _CORE_REDPASS_H_

#include "postprocessing.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: REDPass (render red screen postprocessing pass)
////////////////////////////////////////////////////////////////////////////////

static const char* REDPass_source =
"#version 330 core" "\n"
"in vec2 uv;" "\n"
"out vec4 fragColor;" "\n"
"void main()" "\n"
"{" "\n"
	"fragColor = vec4(1.0, 0.0, 0.0, 1.0);" "\n"
"}";

class REDPass : public PPEffect
{
public:
	REDPass()
		: PPEffect(REDPass_source, PPEFFECT_STR)
	{
	}

	void BindUniforms()
	{
	}
};

#endif