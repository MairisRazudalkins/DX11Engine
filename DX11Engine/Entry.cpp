#include "CoreMinimal.h"
#include "Application.h"
#include "Buffer.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Application* application = nullptr;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
   // application = new Application(hInstance, nCmdShow);

    // Register the window class.
    //const wchar_t CLASS_NAME[] = L"EngineWindowClass";
    //
    //WNDCLASS wc = {};
    //
    //wc.lpfnWndProc = WindowProc;
    //wc.hInstance = hInstance;
    //wc.lpszClassName = CLASS_NAME;
    
    //RegisterClass(&wc);

    Application::CreateApp(hInstance, nCmdShow, application);

    //MSG msg = { };
    //while (GetMessage(&msg, NULL, 0, 0))
    //{
    //    TranslateMessage(&msg);
    //    DispatchMessage(&msg);
    //}
    
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}