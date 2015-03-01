#ifndef _CORE_AATECHNIQUE_H_
#define _CORE_AATECHNIQUE_H_

#include "globals.h"
#include "postprocessing.h"
#include "rt.h"
#include "core.h"
#include "world.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: AATechnique (base class for all antialiasing techniques)
////////////////////////////////////////////////////////////////////////////////

class AATechnique
{
public:
	AATechnique()
	{
	}

	virtual void Draw(World* world)
	{
        RenderTarget::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (world)
			world->Draw(0);
	}

	virtual ~AATechnique()
	{
	}
};

#endif