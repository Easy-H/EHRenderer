#pragma once;

const int NUM_LIGHTS = 4;

#include "../ShaderBase.hpp"
#include <directxmath.h>
#include <wrl.h>

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class LightShaderClass : public ShaderBase {
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	struct LightBufferType {
		XMFLOAT3 lightDirection;
		float padding;
	};

public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();
	
	bool Initialize(ID3D11Device*, HWND);
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3);
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3); 
	void RenderShader(ID3D11DeviceContext*, int);

	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _layout;
	ComPtr<ID3D11SamplerState> _sampleState;
	ComPtr<ID3D11Buffer> _matrixBuffer;

	ComPtr<ID3D11Buffer> _lightBuffer;

};