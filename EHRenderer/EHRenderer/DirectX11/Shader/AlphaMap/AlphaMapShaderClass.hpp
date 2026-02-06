#pragma once

#include "../DX11ShaderBase.hpp"
#include <directxmath.h>
#include <wrl.h>
#include <d3d11.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class AlphaMapShaderClass : public DX11ShaderBase {
private:
	struct MatrixBufferType {
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
public:
	AlphaMapShaderClass();
	AlphaMapShaderClass(const AlphaMapShaderClass&);
	~AlphaMapShaderClass();

	virtual bool Initialize(ID3D11Device* device, HWND hwnd) override;
	virtual bool Render(int indexCount, const Transform* position) override;

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11InputLayout> _layout;
	ComPtr<ID3D11Buffer> _matrixBuffer;
	ComPtr<ID3D11SamplerState> _sampleState;


};