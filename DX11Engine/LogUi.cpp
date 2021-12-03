#include "CoreMinimal.h"
#include "LogUi.h"

#include "Graphics.h"
#include "ImGui/imgui.h"

LogUi::LogUi() : UI() // Using ImGui log example
{
	ClearLog();
}

void LogUi::Render()
{
	int width = Graphics::GetWindowWidth(), height = Graphics::GetWindowHeight();

	ImGui::SetNextWindowSize(ImVec2(width, 300.f), ImGuiCond_Once);
	ImGui::Begin("Logger", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowPos(ImVec2(0.f, height - ImGui::GetWindowHeight()));

	// Main window

	winHeight = ImGui::GetWindowHeight();

	bool clear = ImGui::Button("Clear");
	ImGui::SameLine();

	if (clear)
		ClearLog();

	filter.Draw("Filter", -100.0f);

	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	const char* bufferStart = logBuffer.begin();
	const char* bufferEnd = logBuffer.end();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	ImGuiListClipper clipper;
	clipper.Begin(lineOffsets.Size);
	while (clipper.Step())
	{
		for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
		{
			const char* line_start = bufferStart + lineOffsets[line_no];
			const char* line_end = (line_no + 1 < lineOffsets.Size) ? (bufferStart + lineOffsets[line_no + 1] - 1) : bufferEnd;
			ImGui::TextUnformatted(line_start, line_end);
		}
	}
	clipper.End();

	ImGui::PopStyleVar();

	if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();

	ImGui::End();
}

void LogUi::AddLog(std::string logMsg)
{
	int logBufferSize = logBuffer.size();

	logBuffer.appendf("%s\n", logMsg.c_str());

	for (int size = logBuffer.size(); logBufferSize < size; logBufferSize++)
		if (logBuffer[logBufferSize] == '\n')
			lineOffsets.push_back(logBufferSize + 1);
}

void LogUi::ClearLog()
{
	logBuffer.clear();
	lineOffsets.clear();
	lineOffsets.push_back(0);
}