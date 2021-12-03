#pragma once

class SceneObject;

class PropertyManagerUi : public UI
{
	static PropertyManagerUi* inst;

	SceneObject* activeObj;

	PropertyManagerUi();
	virtual ~PropertyManagerUi();

	void RenderTransformProperty();

public:

	void Render() override;

	static PropertyManagerUi* GetInst() { return inst == nullptr ? inst = new PropertyManagerUi() : inst; }

	void EditProperties(SceneObject* obj);
};