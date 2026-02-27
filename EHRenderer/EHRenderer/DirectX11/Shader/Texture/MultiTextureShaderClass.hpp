#pragma once

#include "../DX11ShaderBase.hpp"
#include <directxmath.h>
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace DirectX;

class MultiTextureShaderClass : public DX11ShaderBase {
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
	virtual bool Render(int indexCount, const Transform* position) override; 
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);
private:
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _layout;
	ComPtr<ID3D11Buffer> _matrixBuffer;
	ComPtr<ID3D11SamplerState> _sampleState;

};