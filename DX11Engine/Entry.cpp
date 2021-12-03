#include "CoreMinimal.h"
#include "Application.h"

Application* application = nullptr;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Application::CreateApp(hInstance, nCmdShow, application);

    return 0;
}