#ifndef _CORE_GLOBALS_H_
#define _CORE_GLOBALS_H_

#include "vec2.h"
#include "world.h"
#include "AATechnique.h"
#include "rt.h"
#include "postprocessing.h"

////////////////////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////////////////////

vec2
	g_screenSizeV2(0, 0);
World
	*g_World			= NULL;
AATechnique
	*g_leftAA			= NULL,
	*g_rightAA			= NULL;
PPEffect
	*g_ppCompare		= NULL,
	*g_ppDisplayThree	= NULL;
RenderTarget
	*g_leftRT			= NULL,
	*g_centerRT			= NULL,
	*g_rightRT			= NULL;

#endif