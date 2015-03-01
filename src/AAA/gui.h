#ifndef _CORE_GUI_H_
#define _CORE_GUI_H_

#include "core.h"
#include "globals.h"
#include "timer.h"
#include "postprocessing.h"

#include "AntTweakBar.h"

#include "AATechnique.h"
#include "pgaa.h"
#include "fxaa.h"
#include "msaa.h"
#include "ssaa.h"

////////////////////////////////////////////////////////////////////////////////
// GUI callbacks
////////////////////////////////////////////////////////////////////////////////

typedef enum { NO_AA, SSAA, MSAA, PGAA, FXAA } AA_TYPE;
TwEnumVal aatypeEV[] = { { NO_AA, "No AA" }, { SSAA, "SSAA" }, { MSAA, "MSAA" }, { PGAA, "PGAA" }, { FXAA, "FXAA" } };
TwType aaType;

static void TW_CALL CompareBtn_callback(void *clientData)
{
    // do something
}

////////////////////////////////////////////////////////////////////////////////
// Class name: GUI (GUI class, holds all GUI setup and callbacks)
////////////////////////////////////////////////////////////////////////////////

class GUI
{
public:
	static TwBar
		*leftBar,
		*rightBar,
		*mainBar;
	static AA_TYPE
        leftAAOld,
		leftAA,
        rightAAOld,
		rightAA;

	static void Init()
	{
		TwInit(TW_OPENGL, NULL);
		aaType = TwDefineEnum("AAType", aatypeEV, 5);

		// main GUI
		mainBar = TwNewBar("App");
		TwDefine(" App size='200 400' ");
		TwDefine(" App position='1210 10' ");
        TwDefine(" App color='0 0 0' ");
        TwDefine(" App alpha='170' ");
        TwAddVarRO(mainBar, "FPS", TW_TYPE_DOUBLE, &Timer::FPS, " group=Information label='FPS:' ");
		TwAddVarRW(mainBar, "Technique1", aaType, &leftAA, " group=Anti-aliasing label='1st tech:' ");
		TwAddVarRW(mainBar, "Technique2", aaType, &rightAA, " group=Anti-aliasing label='2nd tech' ");
        TwAddButton(mainBar, "CompareBtn", CompareBtn_callback, 0, " group='Compare panel' label='Toggle magnifier' ");

		// 1st tech GUI
		leftBar = TwNewBar("Technique_one");
		TwDefine(" Technique_one resizable=false ");
        TwDefine(" Technique_one color='0 0 0' ");
        TwDefine(" Technique_one alpha='170' ");
		TwDefine(" Technique_one size='250 150' ");
		TwDefine(" Technique_one position='10 10' ");

		// 2nd tech GUI
		rightBar = TwNewBar("Technique_two");
		TwDefine(" Technique_two resizable=false ");
        TwDefine(" Technique_two color='0 0 0' ");
        TwDefine(" Technique_two alpha='170' ");
		TwDefine(" Technique_two size='250 150' ");
		TwDefine(" Technique_two position='610 10' ");

	}

    static void switchAA(uint16_t tech, AA_TYPE oldAA, AA_TYPE newAA)
    {
#define GUI_CHOOSE(a,b) tech == 1 ? (a) : (b)

        TwBar* barP         = GUI_CHOOSE(leftBar, rightBar);
        AATechnique* techP  = GUI_CHOOSE(g_leftAA, g_rightAA);

	    // remove old parts
        TwRemoveAllVars(barP);

	    // add new parts and reconfigure technique
        delete techP;
	    switch (newAA)
	    {
		    case NO_AA:
                techP = new AATechnique;
                GUI_CHOOSE(TwDefine(" Technique_one visible=false "), TwDefine(" Technique_two visible=false "));
			    break;
            case SSAA:
                techP = new SSAATechnique(1);
                GUI_CHOOSE(TwDefine(" Technique_one visible=true "), TwDefine(" Technique_two visible=true "));
                TwAddVarRW(barP, "Size", TW_TYPE_UINT16, &((SSAATechnique*)techP)->size, " min=1 max=4 step=1 group=SSAA label='Size' ");
                break;
		    case MSAA:
                techP = new MSAATechnique(1);
                GUI_CHOOSE(TwDefine(" Technique_one visible=true "), TwDefine(" Technique_two visible=true "));
                TwAddVarRW(barP, "Samples", TW_TYPE_UINT32, &((MSAATechnique*)techP)->samples, " min=1 max=16 step=1 group=MSAA label='Samples' ");
			    break;
		    case PGAA:
                techP = new PGAATechnique;
                GUI_CHOOSE(TwDefine(" Technique_one visible=false "), TwDefine(" Technique_two visible=false "));
			    break;
		    case FXAA:
                techP = new FXAATechnique;
                GUI_CHOOSE(TwDefine(" Technique_one visible=false "), TwDefine(" Technique_two visible=false "));
                // TODO
			    break;
	    }
        GUI_CHOOSE(g_leftAA = techP, g_rightAA = techP);
    }

	static void Update()
	{
		// check for changes
        if (leftAA != leftAAOld)
        {
            switchAA(1, leftAAOld, leftAA);
            leftAAOld = leftAA;
        }
        if (rightAA != rightAAOld)
        {
            switchAA(2, rightAAOld, rightAA);
            rightAAOld = rightAA;
        }
		TwDraw();
	}

	static void Destroy()
	{
		TwTerminate();
	}
};

TwBar* GUI::leftBar		= NULL;
TwBar* GUI::rightBar	= NULL;
TwBar* GUI::mainBar		= NULL;

AA_TYPE GUI::leftAAOld  = SSAA;
AA_TYPE GUI::leftAA		= NO_AA;
AA_TYPE GUI::rightAAOld = SSAA;
AA_TYPE GUI::rightAA	= NO_AA;

#endif