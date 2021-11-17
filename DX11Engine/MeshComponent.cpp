#include "CoreMinimal.h"
#include "MeshComponent.h"

void MeshComponent::Render()
{
	ID3D11DeviceContext* context = Graphics::GetDeviceContext();

	UINT stride = sizeof(VertexData::SimpleVertex), offset = 0;

	ID3D11Buffer* vBuffer = mesh->vBuffer->GetBuffer();
	ID3D11Buffer* iBuffer = mesh->iBuffer->GetBuffer();

	// TODO: Assign vShader and pShader

	context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	context->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R16_UINT, 0);
	context->DrawIndexed(mesh->iBuffer->GetBufferSize(), 0, 0);
}
