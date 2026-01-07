#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class RenderTextureClass {
public:
	RenderTextureClass();
	RenderTextureClass(const RenderTextureClass&);
	~RenderTextureClass();

	bool Initialize(ID3D11Device*, int, int, float, float, int);

	void SetRenderTarget(ID3D11DeviceContext*);
	void ClearRenderTarget(ID3D11DeviceContext*, float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();

	void GetProjectionMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	int GetTextureWidth();
	int GetTextureHeight();

private:
	int _textureWidth, _textureHeight;
	ComPtr<ID3D11Texture2D> _renderTargetTexture;
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
	ComPtr<ID3D11Texture2D> _depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	D3D11_VIEWPORT _viewport;
	XMMATRIX _projectionMatrix;
	XMMATRIX _orthoMatrix;
};