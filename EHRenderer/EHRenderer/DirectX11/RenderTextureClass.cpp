#include "RenderTextureClass.hpp"

RenderTextureClass::RenderTextureClass()
{
}

RenderTextureClass::RenderTextureClass(const RenderTextureClass&)
{
}

RenderTextureClass::~RenderTextureClass()
{
}

bool RenderTextureClass::Initialize(ID3D11Device* device,
	int textureWidth, int textureHeight, float screenDepth, float screenNear, int format)
{
	DXGI_FORMAT textureFormat;
	switch (format) {
	case 1:
		textureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	default:
		textureFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	}

	_textureWidth = textureWidth;
	_textureHeight = textureHeight;

	D3D11_TEXTURE2D_DESC textureDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = textureFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr,
		_renderTargetTexture.GetAddressOf()))) {
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateRenderTargetView(_renderTargetTexture.Get(),
		&renderTargetViewDesc, _renderTargetView.GetAddressOf()))) {
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (FAILED(device->CreateShaderResourceView(_renderTargetTexture.Get(),
		&shaderResourceViewDesc, _shaderResourceView.GetAddressOf())))
		return false;

	D3D11_TEXTURE2D_DESC depthBufferDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&depthBufferDesc, nullptr,
		_depthStencilBuffer.GetAddressOf()))) {
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateDepthStencilView(_depthStencilBuffer.Get(),
		&depthStencilViewDesc, _depthStencilView.GetAddressOf())))
	{
		return false;
	}

	_viewport.Width = (float)textureWidth;
	_viewport.Height = (float)textureHeight;
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;

	_projectionMatrix = XMMatrixPerspectiveFovLH((3.141592654f / 4.0f),
		((float)textureWidth / (float)textureHeight), screenNear, screenDepth);
	_orthoMatrix = XMMatrixOrthographicLH((float)textureWidth, (float)textureHeight,
		screenNear, screenDepth);
	
	return true;
}

void RenderTextureClass::SetRenderTarget(ID3D11DeviceContext* deviceContext)
{
	deviceContext->OMSetRenderTargets(1,
		_renderTargetView.GetAddressOf(), _depthStencilView.Get());

	deviceContext->RSSetViewports(1, &_viewport);
}

void RenderTextureClass::ClearRenderTarget(ID3D11DeviceContext* deviceContext,
	float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	deviceContext->ClearRenderTargetView(_renderTargetView.Get(), color);
	deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
}

ID3D11ShaderResourceView* RenderTextureClass::GetShaderResourceView()
{
	return _shaderResourceView.Get();
}

void RenderTextureClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = _projectionMatrix;
}

void RenderTextureClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = _orthoMatrix;
}

int RenderTextureClass::GetTextureWidth()
{
	return _textureWidth;
}

int RenderTextureClass::GetTextureHeight()
{
	return _textureHeight;
}
