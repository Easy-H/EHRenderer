#pragma once

#include "../ShaderBase.hpp"

#include <directxmath.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class ClipPlaneShaderClass : public ShaderBase {
	struct MatrixBufferType {
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
	struct ClipPlaneBufferType {
		XMFLOAT4 clipPlane;
	};
public:
	ClipPlaneShaderClass();
	ClipPlaneShaderClass(const ClipPlaneShaderClass&);
	~ClipPlaneShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND) override;
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _layout;
	ComPtr<ID3D11Buffer> _matrixBuffer;
	ComPtr<ID3D11Buffer> _clipPlaneBuffer;
	ComPtr<ID3D11SamplerState> _sampleState;
};