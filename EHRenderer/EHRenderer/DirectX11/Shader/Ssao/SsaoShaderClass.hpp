#pragma once;

#include "../ShaderBase.hpp"
#include <wrl.h>
#include <directxmath.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class SsaoShaderClass : public ShaderBase {
private:
	struct MatrixBufferType {
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
	struct SsaoBufferType {
		float screenWidth;
		float screenHeight;
		float randomTextureSize;
		float sampleRadius;
		float ssaoScale;
		float ssaoBias;
		float ssaoIntensity;
		float padding;

	};
public:
	SsaoShaderClass();
	SsaoShaderClass(const SsaoShaderClass&);
	~SsaoShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND) override;
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		float, float, float, float, float, float, float);
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		float, float, float, float, float, float, float);
	void RenderShader(ID3D11DeviceContext*, int);

	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11Buffer> _matrixBuffer;
	ComPtr<ID3D11Buffer> _ssaoBuffer;
	ComPtr<ID3D11InputLayout> _inputLayout;
	ComPtr<ID3D11SamplerState> _sampleStateWrap;
	ComPtr<ID3D11SamplerState> _sampleStateClamp;
};