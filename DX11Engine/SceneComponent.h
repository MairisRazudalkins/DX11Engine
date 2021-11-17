#pragma once

class SceneComponent : public Object // maybe add a friend class for a graphics class that handles all rendering operations to call Render on components
{
	

protected:
	DirectX::XMFLOAT4X4 worldMatrix;

	virtual void Render() = 0;
};