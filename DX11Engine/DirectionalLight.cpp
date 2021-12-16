#include "CoreMinimal.h"
#include "DirectionalLight.h"

#include "Buffer.h"

DirectionalLight* DirectionalLight::inst = nullptr;

DirectionalLight::DirectionalLight() : Light()
{
	dirLightCB.lightDirection = DirectX::XMFLOAT3(lightDir.x, lightDir.y, lightDir.z);
	dirLightCB.ambientColor = DirectX::XMFLOAT4(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a);
	dirLightCB.diffuseColor = DirectX::XMFLOAT4(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
	dirLightCB.ambientColor = DirectX::XMFLOAT4(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a);

	dirLightBuffer = Buffer::CreateConstBuffer<DirectionalLightConstBuffer>();

	UpdateConstBuffers();
}

DirectionalLight::~DirectionalLight()
{
	delete dirLightBuffer;
	delete inst;

	inst = nullptr;
}

void DirectionalLight::UpdateConstBuffers()
{
	dirLightCB.diffuseColor = DirectX::XMFLOAT4(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
	dirLightCB.ambientColor = DirectX::XMFLOAT4(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a);
	dirLightCB.specularColor = DirectX::XMFLOAT4(specularColor.r, specularColor.g, specularColor.b, specularColor.a);
	dirLightCB.lightDirection = DirectX::XMFLOAT3(lightDir.x, lightDir.y, lightDir.z);

	Graphics::GetDeviceContext()->UpdateSubresource(dirLightBuffer->GetBuffer(), 0, nullptr, &dirLightCB, 0, 0);
}
