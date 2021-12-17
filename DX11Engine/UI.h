#pragma once

class UI
{

protected:
	bool bIsVisible = true;

public:
	UI() = default;
	virtual ~UI() = default;

	virtual void Render() = 0;

	void SetVisibility(bool bIsVisible) { this->bIsVisible = bIsVisible; }
	bool IsVisible() { return bIsVisible; }
};