#include "CoreMinimal.h"
#include "WaterPlane.h"
#include "LitShader.h"

WaterPlane::WaterPlane(const int numX, const int numY, Transform transfrom) : Plane(numX, numY, transform, new LitShader(this)) // TODO: REPLACE SHADER WITH WATER SHADER
{
}

WaterPlane::~WaterPlane()
{

}