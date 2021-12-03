#pragma once
#include "Ui.h"
#include "ImGui/imgui.h"

class LogUi : public UI
{
	ImGuiTextBuffer logBuffer;
	ImGuiTextFilter filter;
	ImVector<int> lineOffsets;
	bool autoScroll;

public:
	LogUi();

	void Render() override;

	void AddLog(std::string logMsg);
	void ClearLog();

};
