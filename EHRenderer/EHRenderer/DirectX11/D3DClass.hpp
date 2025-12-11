#pragma once;

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class D3DClass {
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void SetBackBufferRenderTarget();
	void ResetViewport();

	void TurnZBufferOn();
	void TurnZBufferOff();

	void EnableAlphaBlending();
	void DisableAlphaBlending();

private:
	bool _vsync_enabled;
	int _videoCardMemory;
	char _videoCardDescription[128];

	ComPtr<IDXGISwapChain> _swapChain;
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	ComPtr<ID3D11Texture2D> _depthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> _depthStencilState;
	ComPtr<ID3D11DepthStencilState> _depthDisabledStencilState;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	ComPtr<ID3D11RasterizerState> _rasterState;

	XMMATRIX _projectionMatrix;
	XMMATRIX _worldMatrix;
	XMMATRIX _orthoMatrix;
	D3D11_VIEWPORT _viewport;

	ComPtr<ID3D11BlendState> _alphaEnableBlendingState;
	ComPtr<ID3D11BlendState> _alphaDisableBlendingState;

};