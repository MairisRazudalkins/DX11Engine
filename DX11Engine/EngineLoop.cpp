#include "CoreMinimal.h"
#include "EngineLoop.h"
#include "Graphics.h"
#include "Input.h"
#include "Time.h"

#include "FileImporter.h"

EngineLoop* EngineLoop::inst = nullptr;

void EngineLoop::Initialize(Application* app, int nCmdShow)
{
    FileImporter::OpenExplorerDialogue(); //DEBUG (remove) 

    this->app = app;

    cam = new Camera(Vector3(0.f, 10.f, -10.f));

    graphics = Graphics::GetInst();
    graphics->Initialize(nCmdShow);
}

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

void EngineLoop::StartEngineLoop()
{
    MSG msg = { };

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            this->Update();
            graphics->Render();
        }
    }
}

LRESULT EngineLoop::ProcessWindowsEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INPUT: // directly from win docs: https://docs.microsoft.com/en-us/windows/win32/dxtecharts/taking-advantage-of-high-dpi-mouse-movement
        {
            UINT dwSize = sizeof(RAWINPUT);
            static BYTE lpb[sizeof(RAWINPUT)];

            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

            RAWINPUT* raw = (RAWINPUT*)lpb;

            if (raw->header.dwType == RIM_TYPEMOUSE)
                Input::GetInst()->RecieveRawMouseInput(raw->data.mouse.lLastX, raw->data.mouse.lLastY);

            break;
        }

    	case WM_CHAR:
		{
			if (lParam & 0x40000000) Input::GetInst()->RecieveInput(static_cast<unsigned char>(wParam), KeyState::Repeat); // check if key is repeating 
            else Input::GetInst()->RecieveInput(static_cast<unsigned char>(wParam), KeyState::Pressed);

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

        case WM_RBUTTONDOWN:
        {
            Input::GetInst()->RecieveInput(VK_RBUTTON, KeyState::Pressed);
            break;
        }

        case WM_RBUTTONUP:
        {
            Input::GetInst()->RecieveInput(VK_RBUTTON, KeyState::Released);
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
            EndPaint(hwnd, &ps);
        }

    return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void EngineLoop::Update()
{
    static float t = 0.0f;

    float deltaTime = Time::GetInst()->Tick();

	//DirectX::XMStoreFloat4x4(&Graphics::GetInst()->_world, DirectX::XMMatrixTranslation(0.f, 0.f, 0.f) * DirectX::XMMatrixRotationY(t));

}