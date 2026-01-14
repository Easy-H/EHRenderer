#pragma once

#include "../ShaderBase.hpp"

#include <directxmath.h>
#include <fstream>
#include <wrl.h>

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

class FireShaderClass : public ShaderBase {
private:
	struct MatrixBufferType {
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
	struct NoiseBufferType {
		float frameTime;
		XMFLOAT3 scrollSpeeds;
		XMFLOAT3 scales;
		float padding;
	};

	struct DistortionBufferType {
		XMFLOAT2 distortion1;
		XMFLOAT2 distortion2;
		XMFLOAT2 distortion3;
		float distortionScale;
		float distortionBias;

	};

public:
	FireShaderClass();
	FireShaderClass(const FireShaderClass&);
	~FireShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND) override;
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float,
		XMFLOAT3, XMFLOAT3, XMFLOAT2, XMFLOAT2, XMFLOAT2, float, float);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX,
		ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, float,
		XMFLOAT3, XMFLOAT3, XMFLOAT2, XMFLOAT2, XMFLOAT2, float, float);

	void RenderShader(ID3D11DeviceContext*, int);

private:
	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _layout;
	ComPtr<ID3D11Buffer> _matrixBuffer;
	ComPtr<ID3D11Buffer> _noiseBuffer;
	ComPtr<ID3D11Buffer> _distortionBuffer;
	ComPtr<ID3D11SamplerState> _sampleStateWrap;
	ComPtr<ID3D11SamplerState> _sampleStateClamp;
};
