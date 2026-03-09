#ifndef _LIGHTSHADERSTAREGYBASE_H_
#define _LIGHTSHADERSTAREGYBASE_H_

#include "ShaderStrategyBase.hpp"
#include <DirectXMath.h>
#include <memory>

using namespace DirectX;


class LightShaderStrategy : public ShaderStrategyBase {
	struct LightBufferType {
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};
public:
	LightShaderStrategy();
	virtual int Bind(ShaderBaseBase* targetShader) override;
	virtual void SetSlot(ShaderBaseBase* targetShader, int slotIdx) override;
private:
	std::unique_ptr<LightBufferType> _buffer;
};

#endif