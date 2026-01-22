#pragma once

#include "../ShaderBase.hpp"

#include <directxmath.h>
#include <fstream>
#include <wrl.h>

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class ShadowShaderClass : public ShaderBase {
private:
	struct MatrixBufferType {
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;

		XMMATRIX lightView;
		XMMATRIX lightProjection;
	};

	struct LightPositionBufferType {
		XMFLOAT3 lightPosition;
		float padding;
	};

	struct LightBufferType {
		float bias;
		XMFLOAT3 padding;

	};

public:
	ShadowShaderClass();
	ShadowShaderClass(const ShadowShaderClass&);
	~ShadowShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		XMFLOAT3, float);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		XMFLOAT3, float);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _layout;

	ComPtr<ID3D11SamplerState> _sampleStateClamp;
	ComPtr<ID3D11SamplerState> _sampleStateWrap;

	ComPtr<ID3D11Buffer> _matrixBuffer;

	ComPtr<ID3D11Buffer> _lightPositionBuffer;
	ComPtr<ID3D11Buffer> _lightBuffer;
};
