#pragma once

#include "../ShaderBase.hpp"
#include <directxmath.h>
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace DirectX;

class MultiTextureShaderClass : ShaderBase {
private:
	struct MatrixBufferType {
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	MultiTextureShaderClass();
	MultiTextureShaderClass(const MultiTextureShaderClass&);
	~MultiTextureShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);
private:
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _layout;
	ComPtr<ID3D11Buffer> _matrixBuffer;
	ComPtr<ID3D11SamplerState> _sampleState;


};