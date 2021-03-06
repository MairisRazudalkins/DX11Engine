#include "CoreMinimal.h"
#include "Time.h"
#include "Graphics.h"

Time* Time::inst = nullptr;

float Time::Tick()
{
    static float t = 0.f;

    if (Graphics::GetDriverType() == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)DirectX::XM_PI * 0.0125f;
    }
    else
    {
        //static DWORD dwTimeStart = 0;
        //DWORD dwTimeCur = GetTickCount();
        //
        //if (dwTimeStart == 0)
        //    dwTimeStart = dwTimeCur;
        //
        //float t = (dwTimeCur - dwTimeStart) / 1000.0f;
        //
        //return t;

        static float prevTime = 0.f;
        deltaTime = (t = (float)GetTickCount() - prevTime) / 1000.f;

        prevTime = (float)GetTickCount();

        return t;
    }

    return t;
}