#include "LightShaderStrategy.hpp"
#include "../../../Data/ShaderBase.hpp"
#include "../../DX11RE.hpp"

#include "../../Object/LightClass.hpp"

LightShaderStrategy::LightShaderStrategy()
{
	_buffer = std::make_unique<LightBufferType>();
}

int LightShaderStrategy::Bind(ShaderBaseBase* targetShader)
{
	std::string name = "LightBuffer";

	return targetShader->GetSlotIdx(name);
}

void LightShaderStrategy::SetSlot(ShaderBaseBase* targetShader, int slotIdx)
{
	LightClass light;
	DX11RE::GetInstance().GetLight(light, 1);

	_buffer->diffuseColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	_buffer->lightDirection = light.GetDirection();
	_buffer->padding = 0;

	//targetShader->ResetSlot(slotIdx, sizeof(LightBufferType));

	targetShader->SetSlot(slotIdx, 0, _buffer.get(), sizeof(LightBufferType));

}