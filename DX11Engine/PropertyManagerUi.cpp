#include "CoreMinimal.h"
#include "PropertyManagerUi.h"
#include "Graphics.h"
#include "SceneObject.h"
#include "DirectionalLight.h"

PropertyManagerUi* PropertyManagerUi::inst = nullptr;

PropertyManagerUi::PropertyManagerUi() : UI()
{

}

PropertyManagerUi::~PropertyManagerUi()
{
	delete inst;
	inst = nullptr;
}

void PropertyManagerUi::Render()
{
	if (!bIsVisible)
		return;

	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Once);
	ImGui::Begin("PropertyManager", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	
	RenderLightingProperty();
	
	ImGui::SetWindowSize(ImVec2(300.f, Graphics::GetWindowHeight() - ImGuiManager::GetInst()->GetLogUI()->GetHeight()));
	ImGui::End();
}

void PropertyManagerUi::EditProperties(SceneObject* obj)
{
	this->activeObj = obj;
}

void PropertyManagerUi::RenderTransformProperty()
{
	float* pos = activeObj->GetPosition().ToArray(); // TODO: TIDY THIS UP
	float* rot = activeObj->GetRotation().ToArray();
	float* scale = activeObj->GetScale().ToArray();

	ImGui::InputFloat3("position", pos, "%.3f");
	ImGui::InputFloat3("rotation", rot, "%.3f");
	ImGui::InputFloat3("Scale", scale, "%.3f");

	activeObj->SetPosition(Vector3(pos[0], pos[1], pos[2]));
	activeObj->SetRotation(Rotator(rot[0], rot[1], rot[2]));
	activeObj->SetScale(Vector3(scale[0], scale[1], scale[2]));

	delete pos;
	delete rot;
	delete scale;
}

void PropertyManagerUi::RenderLightingProperty()
{
	float* dir = DirectionalLight::GetInst()->GetLightDirection().ToArray();
	float* color = DirectionalLight::GetInst()->GetDiffuseColor().ToArray();
	float dayToNightVal = 0.f;

	ImGui::InputFloat3("Light Dir", dir, "%.3f");
	ImGui::ColorEdit3("Light Color", color);
	ImGui::SliderFloat("Day night", &dayToNightVal, 0.f, 100.f, "%.f", 1.f);

	Logger::ENGINE_LOG(Logger::Info, dayToNightVal);

	DirectionalLight::GetInst()->SetLightDirection(Vector3(dir[0], dir[1], dir[2]));
	DirectionalLight::GetInst()->SetDiffuseColor(Color(color[0], color[1], color[2], color[3]));
}