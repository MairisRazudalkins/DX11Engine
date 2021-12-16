#pragma once

class SceneComponent : public Object
{
	

protected:
	DirectX::XMFLOAT4X4 worldMatrix;

public:
	virtual void Render() = 0;
};