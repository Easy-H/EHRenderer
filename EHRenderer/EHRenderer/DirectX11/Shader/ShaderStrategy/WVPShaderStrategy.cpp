#include "WVPShaderStrategy.hpp"
#include "../../../Data/ShaderBase.hpp"

WVPShaderStrategy::WVPShaderStrategy()
{
	_buffer = std::make_unique<MatrixBufferType>();
}

void WVPShaderStrategy::SetWVP(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	_buffer->worldMatrix = XMMatrixTranspose(worldMatrix);
	_buffer->viewMatrix = XMMatrixTranspose(viewMatrix);
	_buffer->projectionMatrix = XMMatrixTranspose(projectionMatrix);
}

int WVPShaderStrategy::Bind(ShaderBaseBase* targetShader)
{
	std::string name = "MatrixBuffer";

	return targetShader->GetSlotIdx(name);
}

void WVPShaderStrategy::SetSlot(ShaderBaseBase* targetShader, int slotIdx)
{
	void* buffer = _buffer.get();

	targetShader->SetSlot(slotIdx, 0, _buffer.get(), sizeof(MatrixBufferType));
}