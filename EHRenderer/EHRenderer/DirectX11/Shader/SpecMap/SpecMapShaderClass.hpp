#pragma once;

#include "../ShaderBase.hpp"
#include <wrl.h>
#include <directxmath.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class SpecMapShaderClass : public ShaderBase {
private:
	struct MatrixBufferType {
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
	struct CameraBufferType {
		XMFLOAT3 cameraPosition;
		float padding;
	};
	struct LightBufferType {
		XMFLOAT4 diffuseColor;
		XMFLOAT4 specularColor;
		float specularPower;
		XMFLOAT3 lightDirection;
	};
public:
	SpecMapShaderClass();
	SpecMapShaderClass(const SpecMapShaderClass&);
	~SpecMapShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND) override;
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		XMFLOAT3, XMFLOAT4, XMFLOAT3, XMFLOAT4, float);
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		XMFLOAT3, XMFLOAT4, XMFLOAT3, XMFLOAT4, float);
	void RenderShader(ID3D11DeviceContext*, int);

	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11Buffer> _matrixBuffer;
	ComPtr<ID3D11Buffer> _cameraBuffer;
	ComPtr<ID3D11Buffer> _lightBuffer;
	ComPtr<ID3D11InputLayout> _inputLayout;
	ComPtr<ID3D11SamplerState> _sampleState;
};