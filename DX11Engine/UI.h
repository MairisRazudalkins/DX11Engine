#pragma once

class UI
{

public:
	UI() = default;
	virtual ~UI() = default;

	virtual void Render() = 0;
};