#pragma once

#include "../ShaderBase.hpp"

#include <directxmath.h>
#include <fstream>
#include <wrl.h>

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class DirectionalLightShadowShaderClass : public ShaderBase {
private:
	struct MatrixBufferType {
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;

		XMMATRIX lightView;
		XMMATRIX lightProjection;
	};

	struct LightBufferType {
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float bias;

	};

public:
	DirectionalLightShadowShaderClass();
	DirectionalLightShadowShaderClass(const DirectionalLightShadowShaderClass&);
	~DirectionalLightShadowShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		XMFLOAT4, XMFLOAT4, XMFLOAT3, float);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		XMFLOAT4, XMFLOAT4, XMFLOAT3, float);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _layout;

	ComPtr<ID3D11SamplerState> _sampleStateClamp;
	ComPtr<ID3D11SamplerState> _sampleStateWrap;

	ComPtr<ID3D11Buffer> _matrixBuffer;

	ComPtr<ID3D11Buffer> _lightBuffer;
};
