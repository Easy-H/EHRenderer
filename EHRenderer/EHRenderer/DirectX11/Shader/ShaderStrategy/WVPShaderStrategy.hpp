#ifndef _WVPSHADERSTAREGYBASE_H_
#define _WVPSHADERSTAREGYBASE_H_

#include "ShaderStrategyBase.hpp"
#include <DirectXMath.h>
#include <memory>

using namespace DirectX;


class WVPShaderStrategy : public ShaderStrategyBase {
	struct MatrixBufferType {
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
public:
	WVPShaderStrategy();
	void SetWVP(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
	virtual int Bind(ShaderBaseBase* targetShader) override;
	virtual void SetSlot(ShaderBaseBase* targetShader, int slotIdx) override;
private:
	std::unique_ptr<MatrixBufferType> _buffer;
};

#endif