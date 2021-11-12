#pragma once
#include "CoreMinimal.h"

class EventHandler
{
	static EventHandler* inst;

	EventHandler() = default;
	~EventHandler() { delete inst; }

};
