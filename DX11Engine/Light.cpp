#include "CoreMinimal.h"
#include "Light.h"

Light::Light() : SceneObject()
{

}

Light::Light(Transform transform, float brightness, Color color) : SceneObject(transform)
{

}
