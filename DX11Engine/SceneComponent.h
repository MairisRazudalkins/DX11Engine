#pragma once
#include "CoreMinimal.h"

class SceneComponent : public Object // maybe add a friend class for a graphics class that handles all rendering operations to call Render on components
{
	

protected:
	virtual void Render() = 0;
};