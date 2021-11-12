
#include "EngineLoop.h"
#include "Application.h"
#include "Input.h"

EngineLoop* EngineLoop::inst = nullptr;

void EngineLoop::Initialize(Application* app, HINSTANCE* hInstance, const wchar_t* className)
{
    WNDCLASS wc = { };
    wc.lpfnWndProc = ProcessWindowsEvent;
    wc.hInstance = *hInstance;
    wc.lpszClassName = className;
    
    RegisterClass(&wc);

    this->app = app;
}

void EngineLoop::StartEngineLoop()
{
    MSG msg = { };

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT EngineLoop::ProcessWindowsEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

        case WM_MOUSEMOVE:
        {
            Input::GetInst()->RecieveMouseMove(LOWORD(lParam), HIWORD(lParam));
            break;
        }

    	case WM_CHAR:
		{
			if (lParam & 0x40000000) // check if key is repeating
                Input::GetInst()->RecieveInput(static_cast<unsigned char>(wParam), KeyState::Repeat);
            else
                Input::GetInst()->RecieveInput(static_cast<unsigned char>(wParam), KeyState::Pressed);

            break;
		}

        case WM_KEYUP:
	    {
            Input::GetInst()->RecieveInput(static_cast<unsigned char>(wParam), KeyState::Released);
            break;
	    }

        case WM_LBUTTONDOWN:
		{
            Input::GetInst()->RecieveInput(VK_LBUTTON, KeyState::Pressed);
            break;
		}

        case WM_LBUTTONUP:
        {
            Input::GetInst()->RecieveInput(VK_LBUTTON, KeyState::Released);
            break;
        }

        case WM_DESTROY:
	    {
		    PostQuitMessage(0);
        	return 0;
	    }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
			BeginPaint(hwnd, &ps);

            GetInst()->app->Render();

            EndPaint(hwnd, &ps);
        }

        GetInst()->app->Update();

    return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}