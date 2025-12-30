#pragma once

#include "../ShaderBase.hpp"
#include <directxmath.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class NormalMapShaderClass : public ShaderBase {
private:
	struct MatrixBufferType {
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
	struct LightBufferType {
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};

public:
	NormalMapShaderClass();
	NormalMapShaderClass(const NormalMapShaderClass&);
	~NormalMapShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND) override;
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
	
private:
	bool InitializeBuffer(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _layout;
	ComPtr<ID3D11Buffer> _matrixBuffer;
	ComPtr<ID3D11Buffer> _lightBuffer;
	ComPtr<ID3D11SamplerState> _sampleState;
};