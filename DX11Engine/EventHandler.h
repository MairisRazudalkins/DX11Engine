#pragma once

class EventHandler
{
	static EventHandler* inst;

	EventHandler() = default;
	~EventHandler() { delete inst; }

};
