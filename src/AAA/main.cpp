/*
#include "core.h"
#include "eventx.h"
#include "camera2d.h"
#include "sprite.h"
#include "math.h"
#include "timer.h"
#include "postprocessing.h"
#include "REDPass.h"
#include "RFTPass.h"
#include "InvertPass.h"
#include "AntTweakBar.h"

#define CONSERVATIVE_RASTERIZATION_NV 0x9346

Camera2D* camera = 0;
Sprite* sprajtovi = 0;
TextureArray* ta = 0;
double t = 0.0;
double dt = 0.000;
HANDLE hStdout;
vec2 vSize;
RenderTarget* fsrt;
InvertPass* rftp;
TwBar *myBar, *mojBar;
float rotSpeed;
int vrrr;
typedef enum { RGB, HSV, CMYK } ColorSpace;
ColorSpace cs = RGB;
TwType colorSpaceType;

void keyUp(unsigned int k)
{
	// exit app
    if (INPUT_KEY_ESC == k)
        Core::Quit();

	// conservative rasterization
	static bool cons_en = false;
	if (INPUT_KEY_C == k)
	{
		(cons_en = !cons_en)
			? glEnable(CONSERVATIVE_RASTERIZATION_NV)
			: glDisable(CONSERVATIVE_RASTERIZATION_NV);
		COORD c; c.X = 0; c.Y = 11;
		SetConsoleCursorPosition(hStdout, c);
		printf("\rConservative rasterization: %s\n", cons_en?"ON ":"OFF");
	}

	// PGAA
	static bool _pgaa = false;
	if (INPUT_KEY_P == k)
	{
		Sprite::setPGAA(_pgaa = !_pgaa);
		COORD c; c.X = 0; c.Y = 13;
		SetConsoleCursorPosition(hStdout, c);
		printf("\rPGAA: %s\n", _pgaa ? "ON " : "OFF");
	}

	// mipmap
	if (INPUT_KEY_M == k)
	{
		Sprite::m_texture->m_minFilter = Sprite::m_texture->m_minFilter == GL_LINEAR
											? GL_LINEAR_MIPMAP_LINEAR
											: GL_LINEAR;
		glBindTexture(GL_TEXTURE_2D_ARRAY, Sprite::m_texture->texture());
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, Sprite::m_texture->m_minFilter);
		GL_ERROR_CHECK;
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		Sprite::setD(camera, vSize);
		COORD c; c.X = 0; c.Y = 9;
		SetConsoleCursorPosition(hStdout, c);
		printf("\rMipmap: %s\n", Sprite::m_texture->m_minFilter == GL_LINEAR ? "OFF" : "ON ");
	}
		
	// animation speed
	static bool paused = false;
	static double old_sp = 0.0;
	if (INPUT_KEY_PAUSE == k)
	{
		paused = !paused;
		if (paused)
		{
			old_sp = dt;
			dt = 0.0;
		}
		else
		{
			dt = old_sp;
		}
	}
	if (INPUT_KEY_UP == k)
	{
		if (!paused)
			dt += 0.003;
	}	
	if (INPUT_KEY_DOWN == k)
	{
		if (!paused)
			dt -= 0.003;
	}
}

void resizeWin(unsigned int w, unsigned int h)
{
    glViewport(0, 0, w, h);
	TwWindowSize(w, h);
}

void TW_CALL RunCB(void *)
{
	COORD c; c.X = 0; c.Y = 7;
	SetConsoleCursorPosition(hStdout, c);
	printf("\rBLABLA\n");
}

void start()
{
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("OpenGL %s (%s)\n", glGetString(GL_VERSION), glGetString(GL_VENDOR));

    Sprite::Init();
    CoreInput::OnKeyUpF = &keyUp;
    CoreInput::OnResizeF = &resizeWin;
    Timer::Init();
	TwInit(TW_OPENGL, NULL);

    // creating camera
	vSize = vec2(Core::GetRect().X,Core::GetRect().Y);
    double size = 100;
    double aspect = Core::GetAspect();
    camera = new Camera2D(size*aspect, size);
	RenderTarget::BackbufferWidth = vSize.x;
	RenderTarget::BackbufferHeight = vSize.y;
#define SS 40

    sprajtovi = new Sprite[SS];
    for (int i = 0; i < SS; i++)
    {
        sprajtovi[i].m_pos = vec2(rand() % (int)(size*aspect) - (int)(size*aspect/2.0), rand() % (int)size-size/2);
        sprajtovi[i].m_scale = vec2(rand()%20+5, rand()%20+5);
		sprajtovi[i].m_layer = i>SS / 2 ? 1 : 0;
		sprajtovi[i].m_rnd = rand() % 200 + 5;
    }

    const char* slike[2] = { RES_FOLDER"/aeolia.bmp", RES_FOLDER"/fotonce.bmp" };
    Sprite::m_texture = TextureArray::loadBMPs(slike, 2, 128, 128, true);

	//const char* slike[2] = { RES_FOLDER"/zombie.bmp", RES_FOLDER"/alien.bmp" };
	//Sprite::m_texture = TextureArray::loadBMPs(slike, 2, 512, 512, true);

	//Sprite::setD(camera, vSize);

	

	// depth testing
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	// blending
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	//glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_DST_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE);	// line-like drawing

	// conservative rasterization
	//keyUp(INPUT_KEY_C);
	// pgaa
	//keyUp(INPUT_KEY_P);
	// mipmap
	//keyUp(INPUT_KEY_M);

	fsrt = new RenderTarget(vSize.x, vSize.y, 24);
	rftp = new InvertPass(fsrt);
	GL_ERROR_CHECK;

    printf("\n\n");

	myBar = TwNewBar("Parameters");
	TwDefine(" Parameters color='0 255 255' ");
	TwDefine(" Parameters movable=false ");
	TwDefine(" Parameters resizable=false ");
	TwDefine(" Parameters size='300 400' ");
	

	mojBar = TwNewBar("Foo");
	TwDefine(" Foo alpha=255 ");
	colorSpaceType = TwDefineEnum("colorSpaceType", NULL, 0);
	TwAddVarRW(mojBar, "Color space:", colorSpaceType, &cs, " enum='0 {RGB}, 1 {HSV}, 2 {CMYk}' ");
	float submarineColor[4] = { 1, 1, 0, 0.5 }; // yellow
	TwAddVarRW(mojBar, "Submarine", TW_TYPE_COLOR4F, &submarineColor, " colormode=rgb ");
	char s1[64] = "a static string";
	TwAddVarRW(mojBar, "falafelio", TW_TYPE_CSSTRING(64*sizeof(char)), s1, "");
	

	TwAddVarRW(myBar, "Broj", TW_TYPE_INT32, &vrrr, " min=-1000 max=1000 group=Engine label='Bla speed' ");
	TwAddVarRW(myBar, "RotSpeed", TW_TYPE_FLOAT, &rotSpeed, " min=0 max=10 step=0.01 group=Engine label='Rotation speed' ");
	TwAddSeparator(myBar, "falafel", " group=Engine ");
	TwAddButton(myBar, "Dugme", RunCB, 0, " key=CTRL+a ");
	TwDefine(" Engine color='255 0 0' ");
}

void run()
{
	//RenderTarget::set(fsrt);
	static COORD c; c.X = 0; c.Y = 15;
	static int fcnt = 0;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < SS; i++)
	{
		//sprajtovi[i].m_pos.y = cos(t + i)*20.0;
		sprajtovi[i].m_angle += dt;
	}
	for (int i = 0; i < SS; sprajtovi[i++].draw(camera, vSize));
	t += dt;

	Timer::Update();
	if (!(fcnt = (fcnt + 1) % 10))
	{
		SetConsoleCursorPosition(hStdout, c);
		printf("FPS: %.1lf     \n", Timer::FPS);
	}

	//RenderTarget::set(0);
	//glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//rftp->Draw();
	GL_ERROR_CHECK;
	glDisable(CONSERVATIVE_RASTERIZATION_NV);
	TwDraw();
	glEnable(CONSERVATIVE_RASTERIZATION_NV);
}

void end()
{
    delete[] sprajtovi;
    delete camera;
    delete ta;
	delete fsrt;
	delete rftp;
    Sprite::Destroy();
	TwTerminate();
}

void(*Core::OnInitF)()  = &start;
void(*Core::OnFrameF)() = &run;
void(*Core::OnExitF)()  = &end;

*/