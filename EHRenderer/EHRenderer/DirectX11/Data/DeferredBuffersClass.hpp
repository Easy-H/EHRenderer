#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>

const int BUFFER_COUNT = 2;

using namespace DirectX;
using namespace Microsoft::WRL;

class DeferredBuffersClass {
public:
	DeferredBuffersClass();
	DeferredBuffersClass(const DeferredBuffersClass&);
	~DeferredBuffersClass();

	bool Initialize(ID3D11Device*, int, int, float, float);

	void SetRenderTargets(ID3D11DeviceContext*);
	void ClearRenderTargets(ID3D11DeviceContext*, float, float, float, float);

	ID3D11ShaderResourceView* GetShaderResourceView(int);

private:
	int _textureWidth;
	int _textureHeight;

	ComPtr<ID3D11Texture2D> _renderTargetTextureArray[BUFFER_COUNT];
	ComPtr<ID3D11RenderTargetView> _renderTargetViewArray[BUFFER_COUNT];
	ComPtr<ID3D11ShaderResourceView> _shaderResourceViewArray[BUFFER_COUNT];
	ComPtr<ID3D11Texture2D> _depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	D3D11_VIEWPORT _viewport;

};