#pragma once

#include <d3d11.h>

class ShaderBase {
public:
	virtual ~ShaderBase() {}

	virtual bool Initialize(ID3D11Device*, HWND) = 0;

protected:
	void OutputShaderErrorMessage(ID3D10Blob* erroMessage, HWND hwnd, WCHAR* filename);

	bool CreateConstantBuffer(ID3D11Device* device, unsigned int size, ID3D11Buffer** target);
	bool CreateSamplerState(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE mode, ID3D11SamplerState** target);

};