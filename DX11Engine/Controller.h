#pragma once
#include "Object.h"

class Controller : public Object
{
protected:
	virtual void SetupInputBinds() = 0;

public:
	Controller() = default;
	virtual ~Controller() = default;
};