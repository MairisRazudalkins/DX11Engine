#include "CoreMinimal.h"
#include "PropertyManagerUi.h"
#include "Graphics.h"
#include "SceneObject.h"

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
	if (activeObj != nullptr)
	{
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Once);

		ImGui::Begin("PropertyManager", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		RenderTransformProperty();

		ImGui::SetWindowSize(ImVec2(300.f, Graphics::GetWindowHeight() - ImGuiManager::GetInst()->GetLogUI()->GetHeight()));

		ImGui::End();
	}
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