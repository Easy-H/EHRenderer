#pragma once

#include <d3d11.h>

class ShaderBase {
public:
	virtual ~ShaderBase() {}
	virtual bool Initialize(ID3D11Device*, HWND) = 0;
	
protected:
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
};