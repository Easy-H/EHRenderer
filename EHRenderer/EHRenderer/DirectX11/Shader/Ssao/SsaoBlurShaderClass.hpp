#pragma once;

#include "../ShaderBase.hpp"
#include <wrl.h>
#include <directxmath.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class SsaoBlurShaderClass : public ShaderBase {
private:
	struct MatrixBufferType {
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};
	struct ScreenBufferType {
		float screenWidth;
		float screenHeight;
		float blurType;
		float padding;

	};
public:
	SsaoBlurShaderClass();
	SsaoBlurShaderClass(const SsaoBlurShaderClass&);
	~SsaoBlurShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND) override;
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		int, int, int);
private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		int, int, int);
	void RenderShader(ID3D11DeviceContext*, int);

	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11Buffer> _matrixBuffer;
	ComPtr<ID3D11Buffer> _screenBuffer;
	ComPtr<ID3D11InputLayout> _inputLayout;
	ComPtr<ID3D11SamplerState> _sampleState;
};