#ifndef _CORE_WORLD_H_
#define _CORE_WORLD_H_

#include "core.h"
#include "shader.h"
#include "sprite.h"
#include "camera2d.h"
#include "globals.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: World (class that holds scene to be drawn and all requried info)
////////////////////////////////////////////////////////////////////////////////

class World
{
public:
	World() {}
	virtual void Update() = 0;
	virtual void Draw(ShaderProgram* overrideProgram) = 0;
	virtual ~World()
	{}
};

////////////////////////////////////////////////////////////////////////////////
// Class name: SpriteWorld (class that draws bunch of sprites)
////////////////////////////////////////////////////////////////////////////////

#define SS 15

class SpriteWorld : public World
{
	Camera2D* camera;
	Sprite* sprajtovi;
public:
	SpriteWorld()
	{
		Sprite::Init();
		// generate shitload of sprites
		double size = 100;
		double aspect = Core::GetAspect();
		camera = new Camera2D(size, size);

		sprajtovi = new Sprite[SS];
		for (int i = 0; i < SS; i++)
		{
			sprajtovi[i].m_pos = vec2(rand() % (int)(size) - (int)(size / 2.0), rand() % (int)size - size / 2);
			sprajtovi[i].m_scale = vec2(rand() % 20 + 5, rand() % 20 + 5);
			sprajtovi[i].m_layer = i>SS / 2 ? 1 : 0;
			sprajtovi[i].m_rnd = rand() % 200 + 5;
		}

		const char* slike[2] = { RES_FOLDER"/aeolia.bmp", RES_FOLDER"/fotonce.bmp" };
		Sprite::m_texture = TextureArray::loadBMPs(slike, 2, 128, 128, true);
	}
	void Update()
	{
		for (int i = 0; i < SS; i++)
		{
			sprajtovi[i].m_angle += 0.003;
		}
	}
	void Draw(ShaderProgram* overrideProgram)
	{
		// draw sprites
        if (overrideProgram)
        {
            for (int i = 0; i < SS; sprajtovi[i++].draw(camera, vec2(600, 600), overrideProgram));
        }
        else
        {
            for (int i = 0; i < SS; sprajtovi[i++].draw(camera, vec2(600, 600)));
        }
	}
	~SpriteWorld()
	{
		// destroy sprites
		delete[] sprajtovi;
		delete camera;
		Sprite::Destroy();
	}
};

#endif