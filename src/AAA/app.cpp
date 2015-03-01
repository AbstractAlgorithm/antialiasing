/* core includes */
#include "core.h"
#include "globals.h"
#include "gui.h"
#include "timer.h"
#include "DiffPass.h"
#include "Canvas3Pass.h"
/* aa techniques */
#include "AATechnique.h"
#include "msaa.h"
#include "ssaa.h"
#include "fxaa.h"

void start()
{
	// OpenGL
    printf("OpenGL %s (%s)\n", glGetString(GL_VERSION), glGetString(GL_VENDOR));

	// Timer
	Timer::Init();
	
	// globals
	g_screenSizeV2		= vec2(Core::GetRect().X, Core::GetRect().Y);
	RenderTarget::BackbufferWidth	= g_screenSizeV2.x;
	RenderTarget::BackbufferHeight	= g_screenSizeV2.y;
	g_World				= new SpriteWorld;
	g_leftAA			= new AATechnique;
	g_rightAA			= new AATechnique;
	g_leftRT			= new RenderTarget(g_screenSizeV2.x / 3, g_screenSizeV2.y);
	g_centerRT			= new RenderTarget(g_screenSizeV2.x / 3, g_screenSizeV2.y);
	g_rightRT			= new RenderTarget(g_screenSizeV2.x / 3, g_screenSizeV2.y);
	g_ppCompare			= new DiffPass(g_leftRT, g_centerRT);
	g_ppDisplayThree	= new Canvas3Pass(g_leftRT, g_centerRT, g_rightRT);

	// GUI
	GUI::Init();
}

void run()
{
	// update scene
	g_World->Update();

	RenderTarget::set(g_leftRT);												// draw first AA technique
    RenderTarget::clearColor(0.3f, 0.3f, 0.3f, 1.0f);
	g_leftAA->Draw(g_World);

	RenderTarget::set(g_centerRT);												// draw second AA tehcnique
    RenderTarget::clearColor(0.3f, 0.3f, 0.3f, 1.0f);
	g_rightAA->Draw(g_World);

	RenderTarget::set(g_rightRT);												// draw difference
	RenderTarget::clearColor(0, 0, 0.3f, 1.0f);
	g_ppCompare->Draw();

	RenderTarget::set(0);														// draw all three screens from render targets
	RenderTarget::clearColor(0, 0, 0, 1.0f);
	g_ppDisplayThree->Draw();

	// update timer
	Timer::Update();

	// update gui
	GUI::Update();
}

void end()
{
	// GUI
	GUI::Destroy();

	// globals
	delete g_World;
	delete g_leftAA;
	delete g_rightAA;
	delete g_leftRT;
	delete g_centerRT;
	delete g_rightRT;
	delete g_ppCompare;
	delete g_ppDisplayThree;
}

void(*Core::OnInitF)()  = &start;
void(*Core::OnFrameF)() = &run;
void(*Core::OnExitF)()  = &end;