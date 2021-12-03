#pragma once
class LogUi;

class ImGuiManager
{
	ImGuiManager();
	ImGuiManager(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~ImGuiManager();

	static ImGuiManager* inst;

	LogUi* logUIInst;

	void CreateLogUI();

public:
	static void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext) { if (!inst) { inst = new ImGuiManager(device, deviceContext ); }}
	static ImGuiManager* GetInst() { return inst != nullptr ? inst : inst = new ImGuiManager(); }

	LogUi* GetLogUI() { return logUIInst; }

	void Render();
};
