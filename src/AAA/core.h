#ifndef _core_h_
#define _core_h_

#define CORE_TITLE L"default title"
#define CORE_WIDTH 1280
#define CORE_HEIGHT 640
#define CORE_FULLSCREEN false
#define RES_FOLDER "_res"

#define DEBUGMODE

#include <stdio.h>
#include <windows.h>
#include <glew.h>
#include "AntTweakBar.h"

#ifdef DEBUGMODE
#define GL_ERROR_CHECK	            \
    do {                            \
        GLenum err = glGetError();	\
        if (err != GL_NO_ERROR)	    \
            printf("ERROR: 0x%x\n%s : %d\n", err, __FILE__, __LINE__); \
    } while (0)

#else
#define GL_ERROR_CHECK do{}while(0)
#endif

#ifdef DEBUGMODE
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif

// commonly used keys
#define INPUT_KEY_ESC 0x1B
#define INPUT_KEY_BACK 0x08
#define INPUT_KEY_TAB 0x09
#define INPUT_KEY_ENTER 0x0D
#define INPUT_KEY_SPACE 0x20
#define INPUT_KEY_PAUSE 0x13
#define INPUT_KEY_CAPS 0x14
#define INPUT_KEY_INSERT 0x2D
#define INPUT_KEY_DELETE 0x2E
// system keys
#define INPUT_KEY_SHIFT 0x10
#define INPUT_KEY_CTRL 0x11
#define INPUT_KEY_ALT 0x12
#define INPUT_KEY_LSHIFT 0xA0
#define INPUT_KEY_RSHIFT 0xA1
#define INPUT_KEY_LCTRL 0xA2
#define INPUT_KEY_RCTRL 0xA3
#define INPUT_KEY_LALT 0xA4
#define INPUT_KEY_RALT 0xA5
// arrow keys
#define INPUT_KEY_LEFT 0x25
#define INPUT_KEY_UP 0x26
#define INPUT_KEY_RIGHT 0x27
#define INPUT_KEY_DOWN 0x28
// number keys
#define INPUT_KEY_0 0x30
#define INPUT_KEY_1 0x31
#define INPUT_KEY_2 0x32
#define INPUT_KEY_3 0x33
#define INPUT_KEY_4 0x34
#define INPUT_KEY_5 0x35
#define INPUT_KEY_6 0x36
#define INPUT_KEY_7 0x37
#define INPUT_KEY_8 0x38
#define INPUT_KEY_9 0x39
// letter keys
#define INPUT_KEY_A 0x41
#define INPUT_KEY_B 0x42
#define INPUT_KEY_C 0x43
#define INPUT_KEY_D 0x44
#define INPUT_KEY_E 0x45
#define INPUT_KEY_F 0x46
#define INPUT_KEY_G 0x47
#define INPUT_KEY_H 0x48
#define INPUT_KEY_I 0x49
#define INPUT_KEY_J 0x4A
#define INPUT_KEY_K 0x4B
#define INPUT_KEY_L 0x4C
#define INPUT_KEY_M 0x4D
#define INPUT_KEY_N 0x4E
#define INPUT_KEY_O 0x4F
#define INPUT_KEY_P 0x50
#define INPUT_KEY_Q 0x51
#define INPUT_KEY_R 0x52
#define INPUT_KEY_S 0x53
#define INPUT_KEY_T 0x54
#define INPUT_KEY_U 0x55
#define INPUT_KEY_V 0x56
#define INPUT_KEY_W 0x57
#define INPUT_KEY_X 0x58
#define INPUT_KEY_Y 0x59
#define INPUT_KEY_Z 0x5A
// function keys
#define INPUT_KEY_F1 0x70
#define INPUT_KEY_F2 0x71
#define INPUT_KEY_F3 0x72
#define INPUT_KEY_F4 0x73
#define INPUT_KEY_F5 0x74
#define INPUT_KEY_F6 0x75
#define INPUT_KEY_F7 0x76
#define INPUT_KEY_F8 0x77
#define INPUT_KEY_F9 0x78
#define INPUT_KEY_F10 0x79
#define INPUT_KEY_F11 0x7A
#define INPUT_KEY_F12 0x7B
// mouse buttons
#define INPUT_MOUSE_LEFT 0x0001
#define INPUT_MOUSE_MIDDLE 0x0010
#define INPUT_MOUSE_RIGHT 0x0002

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

struct CoreInput
{
    static void(*OnMouseMoveF)(int, int, unsigned int, unsigned int);
    static void(*OnMouseDownF)(int);
    static void(*OnMouseUpF)(int);
    static void(*OnMouseDoubleF)(int);
    static void(*OnKeyDownF)(unsigned int);
    static void(*OnKeyUpF)(unsigned int);
    static void(*OnResizeF)(unsigned int, unsigned int);
};

class Core
{
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    HDC m_hDC;
    LPCWSTR m_title;
    bool m_quit;
    struct RenderingContext
    {
        HGLRC m_hRC;
        int m_width, m_height;
        bool m_fullscreen;

        RenderingContext()
            : m_hRC(0)
            , m_width(CORE_WIDTH)
            , m_height(CORE_HEIGHT)
            , m_fullscreen(false)
        {}

        bool Init()
        {
            int format;
            Core* sys = Core::Get();
            static PIXELFORMATDESCRIPTOR pfd =
            {
                sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
                1, // Version Number
                PFD_DRAW_TO_WINDOW | // Format Must Support Window
                PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
                PFD_DOUBLEBUFFER, // Must Support Double Buffering
                PFD_TYPE_RGBA, // Request An RGBA Format
                32, // Select Our Color Depth
                0, 0, 0, 0, 0, 0, // Color Bits Ignored
                0, // No Alpha Buffer
                0, // Shift Bit Ignored
                0, // No Accumulation Buffer
                0, 0, 0, 0, // Accumulation Bits Ignored
                16, // 16Bit Z-Buffer (Depth Buffer)
                0, // No Stencil Buffer
                0, // No Auxiliary Buffer
                PFD_MAIN_PLANE, // Main Drawing Layer
                0, // Reserved
                0, 0, 0
            };
			
            if (!(format = ChoosePixelFormat(sys->m_hDC, &pfd))) printf("Error: PixelFormat fail.\n");
            if (!SetPixelFormat(sys->m_hDC, format, &pfd)) printf("Error: PixelFormat not set.\n");
            if (!(m_hRC = wglCreateContext(sys->m_hDC))) printf("Error: Context not creted.\n");
            if (!wglMakeCurrent(sys->m_hDC, m_hRC)) printf("Error: Context not set.\n");

            return true;
        }
        void Destroy()
        {
            if (m_fullscreen)
            {
                ChangeDisplaySettings(NULL, 0);
                ShowCursor(true);
            }
            if (m_hRC)
            {
                if (!wglMakeCurrent(NULL, NULL))
                    MessageBox(NULL, L"Release Of DC And RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
                if (!wglDeleteContext(m_hRC))
                    MessageBox(NULL, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
                m_hRC = NULL;
            }
        }
    } *m_rc;
    static Core* instance;

    Core(HINSTANCE hi)
        : m_hInstance(hi)
        , m_hWnd(0)
        , m_hDC(0)
        , m_title(CORE_TITLE)
        , m_quit(false)
        , m_rc(0)
    {}

    void loadSettings(int& w, int& h, bool& f)
    {
        FILE* file = fopen(RES_FOLDER"/settings.ini", "r");
        if (!file)
        {
#ifdef DEBUGMODE
            printf("Fajl nije nadjen.\n");
#endif
            return;
        }

        int fs = f;
        fscanf(file, "%d", &w);
        fscanf(file, "%d", &h);
        fscanf(file, "%d", &fs);
        f = fs;

        fclose(file);
    }

    // windows handling
    bool CreateWin(PCWSTR t = CORE_TITLE, int w = CORE_WIDTH, int h = CORE_HEIGHT, bool f = CORE_FULLSCREEN)
    {
#ifdef DEBUGMODE
        printf("Create win: '%ls' (%dx%d).\n", t, w, h);
#endif
        // Determine the resolution of the clients desktop screen.
        unsigned long screenWidth = GetSystemMetrics(SM_CXSCREEN);
        unsigned long screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int posX, posY;

        // Setup the windows class with default settings.
        WNDCLASSEX wc;
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = m_hInstance;
        wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        wc.hIconSm = wc.hIcon;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = L"core_INPUT_winclass";
        wc.cbSize = sizeof(WNDCLASSEX);

        // Register the window class.
        RegisterClassEx(&wc);


        // Setup the screen settings depending on whether it is running in full screen or in windowed mode.
        if (f)
        {
            DEVMODE dmScreenSettings;
            // If full screen set the screen to maximum size of the users desktop and 32bit.
            memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);
            dmScreenSettings.dmPelsWidth = screenWidth;
            dmScreenSettings.dmPelsHeight = screenHeight;
            dmScreenSettings.dmBitsPerPel = 32;
            dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
                return false;

            posX = posY = 0;
        }
        else
        {
            // If windowed then set it to given res
            screenWidth = w;
            screenHeight = h;

            // Place the window in the middle of the screen.
            posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
            posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
        }

        RECT WindowRect;
        WindowRect.left = (long)0;
        WindowRect.right = (long)screenWidth;
        WindowRect.top = (long)0;
        WindowRect.bottom = (long)screenHeight;
        AdjustWindowRectEx(&WindowRect, (f ? WS_POPUP : WS_OVERLAPPEDWINDOW) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, FALSE, f ? WS_EX_APPWINDOW : (WS_EX_APPWINDOW | WS_EX_WINDOWEDGE));

        // Create the window with the screen settings and get the handle to it.
        m_hWnd = CreateWindowEx(
            f ? WS_EX_APPWINDOW : (WS_EX_APPWINDOW | WS_EX_WINDOWEDGE),
            L"core_INPUT_winclass",
            t,
            (f ? WS_POPUP : WS_OVERLAPPEDWINDOW) | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
            (GetSystemMetrics(SM_CXSCREEN) - WindowRect.right + WindowRect.left) / 2,
            (GetSystemMetrics(SM_CYSCREEN) - WindowRect.bottom + WindowRect.top) / 2,
            WindowRect.right - WindowRect.left,
            WindowRect.bottom - WindowRect.top,
            NULL,
            NULL,
            m_hInstance,
            NULL);

        if (!m_hWnd)
            return false;

        if (!(m_hDC = GetDC(m_hWnd)))
            return false;

        m_rc = new RenderingContext;
        m_title = t;
        m_rc->m_width = w;
        m_rc->m_height = h;
        m_rc->m_fullscreen = f;
        if (!m_rc->Init())
            return false;

        // Bring the window up on the screen and set it as main focus.
        ShowWindow(m_hWnd, SW_SHOW);
        SetForegroundWindow(m_hWnd);
        SetFocus(m_hWnd);

        // Hide the mouse cursor.
        //ShowCursor(false);

        return true;
    }
    void DestroyWin()
    {
#ifdef DEBUGMODE
        printf("Destroy win '%ls'.\n", m_title);
#endif

        m_rc->Destroy();

        if (m_hDC && !ReleaseDC(m_hWnd, m_hDC))
        {
            MessageBox(NULL, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
            m_hDC = NULL;
        }

        if (m_hWnd && !DestroyWindow(m_hWnd))
        {
            MessageBox(NULL, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
            m_hWnd = NULL;
        }

        if (!UnregisterClass(L"core_INPUT_winclass", m_hInstance))
        {
            MessageBox(NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
            m_hInstance = NULL;
        }
    }

    static void Create(HINSTANCE hi)
    {
        if (instance == NULL)
            instance = new Core(hi);
    }
    static Core* Get()
    {
        return instance;
    }

    bool Init()
    {
        int w = CORE_WIDTH, h = CORE_HEIGHT;
        bool f = CORE_FULLSCREEN;
        loadSettings(w, h, f);

        if (!CreateWin(CORE_TITLE, w, h, f))
            return false;

        GLenum err = glewInit();
        if (err != GLEW_OK)
            return false;

        return true;
    }

    void Run()
    {
        MSG msg;
        ZeroMemory(&msg, sizeof(MSG));                                          // Initialize the message structure.

        m_quit = false;
        while (!m_quit)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))                       // handle message
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            if (msg.message == WM_QUIT)                                         // if it wants to exit
            {
                m_quit = true;
            }
            else                                                                // otherwise, do frame processing
            {
                if (Core::OnFrameF)
                {
                    (*Core::OnFrameF)();
                    SwapBuffers(m_hDC);
                }
            }
        }

        return;                                                                     // unreachable code
    }

    void Destroy()
    {
        DestroyWin();
        delete m_rc;
    }

    friend int WINAPI WinMain (HINSTANCE, HINSTANCE, LPSTR, int);

public:

    static void(*OnInitF)();
    static void(*OnExitF)();
    static void(*OnFrameF)();

    static void Quit() { instance->m_quit = true; }

    static bool SetWindow(PCWSTR t = CORE_TITLE, int w = CORE_WIDTH, int h = CORE_HEIGHT, bool f = CORE_FULLSCREEN)
    {
        Core::instance->Destroy();
        return Core::instance->CreateWin(t, w, h, f);
    }

    static double GetAspect()
    {
        GLint viewportDims[4];
        glGetIntegerv(GL_VIEWPORT, viewportDims);
        return (double)viewportDims[2] / (double)viewportDims[3];
    }
	static COORD GetRect()
	{
		COORD r;
		GLint viewportDims[4];
		glGetIntegerv(GL_VIEWPORT, viewportDims);
		r.X = (double)viewportDims[2];
		r.Y = (double)viewportDims[3];
		return r;
	}
};

////////////////////////////////////////////////////////////////////////////////
// Class name: CoreInput
////////////////////////////////////////////////////////////////////////////////

void(*CoreInput::OnMouseMoveF)(int, int, unsigned int, unsigned int)    = NULL;
void(*CoreInput::OnMouseDownF)(int)                                     = NULL;
void(*CoreInput::OnMouseUpF)(int)                                       = NULL;
void(*CoreInput::OnMouseDoubleF)(int)                                   = NULL;
void(*CoreInput::OnKeyDownF)(unsigned int)                              = NULL;
void(*CoreInput::OnKeyUpF)(unsigned int)                                = NULL;
void(*CoreInput::OnResizeF)(unsigned int, unsigned int)                 = NULL;

////////////////////////////////////////////////////////////////////////////////
// Class name: Core
////////////////////////////////////////////////////////////////////////////////

Core* Core::instance = NULL;

////////////////////////////////////////////////////////////////////////////////
// MAIN LOOP OF THE APPLICATION
////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wParam, LPARAM lParam)
{
	TwEventWin(hwnd, umessage, wParam, lParam);

    switch (umessage)
    {
        case WM_CREATE:
            return 0;
        case WM_DESTROY:                                                        // window is being destroyed
        {
            //PostQuitMessage(0);
            return 0;
        }
        case WM_CLOSE:                                                          // window is being closed
        {
            PostQuitMessage(0);
            return 0;
        }
        
        case WM_KEYDOWN:
        {
            if (CoreInput::OnKeyDownF)
                (*CoreInput::OnKeyDownF)(wParam);
            return 0;
        }
        case WM_KEYUP:
        {
            if (CoreInput::OnKeyUpF)
                (*CoreInput::OnKeyUpF)(wParam);
            return 0;
        }
        case WM_MOUSEMOVE:
        {
            if (CoreInput::OnMouseMoveF)
                (*CoreInput::OnMouseMoveF)
                    (
                        wParam,
                        ((wParam == INPUT_MOUSE_LEFT || wParam == INPUT_MOUSE_MIDDLE || wParam == INPUT_MOUSE_RIGHT) ? 1 : 0),
                        LOWORD(lParam),
                        HIWORD(lParam)
                    );
            return 0;
        }
        case WM_LBUTTONDOWN:
        {
            if (CoreInput::OnMouseDownF)
                (*CoreInput::OnMouseDownF)(INPUT_MOUSE_LEFT);
            return 0;
        }
        case WM_MBUTTONDOWN:
        {
            if (CoreInput::OnMouseDownF)
                (*CoreInput::OnMouseDownF)(INPUT_MOUSE_MIDDLE);
            return 0;
        }
        case WM_RBUTTONDOWN:
        {
            if (CoreInput::OnMouseDownF)
                (*CoreInput::OnMouseDownF)(INPUT_MOUSE_RIGHT);
            return 0;
        }
        case WM_LBUTTONUP:
        {
            if (CoreInput::OnMouseUpF)
                (*CoreInput::OnMouseUpF)(INPUT_MOUSE_LEFT);
            return 0;
        }
        case WM_MBUTTONUP:
        {
            if (CoreInput::OnMouseUpF)
                (*CoreInput::OnMouseUpF)(INPUT_MOUSE_MIDDLE);
            return 0;
        }
        case WM_RBUTTONUP:
        {
            if (CoreInput::OnMouseUpF)
                (*CoreInput::OnMouseUpF)(INPUT_MOUSE_RIGHT);
            return 0;
        }
        case WM_LBUTTONDBLCLK:
        {
            if (CoreInput::OnMouseDoubleF)
                (*CoreInput::OnMouseDoubleF)(INPUT_MOUSE_LEFT);
            return 0;
        }
        case WM_MBUTTONDBLCLK:
        {
            if (CoreInput::OnMouseDoubleF)
                (*CoreInput::OnMouseDoubleF)(INPUT_MOUSE_MIDDLE);
            return 0;
        }
        case WM_RBUTTONDBLCLK:
        {
            if (CoreInput::OnMouseDoubleF)
                (*CoreInput::OnMouseDoubleF)(INPUT_MOUSE_RIGHT);
            return 0;
        }
        case WM_SIZE:
        {
            if (CoreInput::OnResizeF)
                (*CoreInput::OnResizeF)(LOWORD(lParam), HIWORD(lParam));
            return 0;
        }
        default:                                                                // all other messages
        {
            return DefWindowProc(hwnd, umessage, wParam, lParam);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// MAIN ENTRY POINT OF THE APPLICATION
////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain                                                              // main entry point of the app
    (
        HINSTANCE hInstance,                                                    // current app
        HINSTANCE hPrevInstance,                                                // always null
        LPSTR lpCmdLine,                                                        // cmd line arguments
        int nCmdShow                                                            // ShowWindow() param
    )
{
    Core::Create(hInstance);

    Core* system = Core::Get();
    if (!system)
        return 1;

#ifdef DEBUGMODE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

    if (system->Init())                                                         // create win and RC
    {
        if (Core::OnInitF)
            (*Core::OnInitF)();
        system->Run();                                                          // run main message loop
    }

    if (Core::OnExitF)
        (*Core::OnExitF)();
    system->Destroy();                                                          // release resources
    delete system;
    system = 0;

#ifdef DEBUGMODE
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}

#endif