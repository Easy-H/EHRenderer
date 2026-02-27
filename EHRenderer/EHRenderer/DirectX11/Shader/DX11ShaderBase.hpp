#pragma once

#include "../../Data/ShaderBase.hpp"
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

class DX11ShaderBase : public ShaderBaseBase {
public:
	virtual bool Initialize(ID3D11Device* device, HWND hwnd) = 0;
	virtual ~DX11ShaderBase() {}
	virtual void BindTexture(int slot, int textureId) override;
	virtual void GetSlot(const std::string& name, int& slotId, size_t& size) override;
	virtual void SetSlot(int slotId, int slotOffset, void* valuePtr, size_t size) override;
	virtual int GetSlotCnt() override;
	virtual void ResetSlot(int slotId, size_t size) override;

protected:
	void OutputShaderErrorMessage(ID3D10Blob* erroMessage, HWND hwnd, WCHAR* filename);
	void GetXMMATRIX(const Transform* position, XMMATRIX& matrix);

	bool CreateConstantBuffer(ID3D11Device* device, unsigned int size, ID3D11Buffer** target);
	bool CreateSamplerState(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE mode, ID3D11SamplerState** target);

};