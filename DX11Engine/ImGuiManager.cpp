#include "CoreMinimal.h"
#include "ImGuiManager.h"

#include "LogUi.h"

#include "Application.h"
#include "DirectionalLight.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

ImGuiManager* ImGuiManager::inst = nullptr;

ImGuiManager::ImGuiManager()
{
	CreateLogUI();
}

ImGuiManager::ImGuiManager(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO io = ImGui::GetIO();
	ImGui_ImplWin32_Init(Application::GetHWND());
	ImGui_ImplDX11_Init(device, deviceContext);
	ImGui::StyleColorsDark();

	CreateLogUI();
}

ImGuiManager::~ImGuiManager()
{
	delete inst;
}

void ImGuiManager::CreateLogUI()
{
	logUIInst = new LogUi();
}

void ImGuiManager::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Directional Light Settings", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);

	Vector3 lightDir = DirectionalLight::GetInst()->GetLightDirection();

	ImGui::SliderFloat("X", &lightDir.x, -2.f, 2.f);
	ImGui::SliderFloat("Y", &lightDir.y, -2.f, 2.f);
	ImGui::SliderFloat("Z", &lightDir.z, -2.f, 2.f);

	DirectionalLight::GetInst()->SetLightDirection(lightDir);

	Color lightColor = DirectionalLight::GetInst()->GetDiffuseColor();

	float color[3] = { lightColor.r, lightColor.g, lightColor.b };

	ImGui::ColorEdit3("LightColor", color);

	DirectionalLight::GetInst()->SetDiffuseColor(Color(color[0], color[1], color[2], 1.f));

	ImGui::End();

	logUIInst->Render();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
