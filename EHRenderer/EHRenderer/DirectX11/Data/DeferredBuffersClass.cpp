#include "DeferredBuffersClass.hpp"

DeferredBuffersClass::DeferredBuffersClass()
{

}

DeferredBuffersClass::DeferredBuffersClass(const DeferredBuffersClass&)
{
}

DeferredBuffersClass::~DeferredBuffersClass()
{
}

bool DeferredBuffersClass::Initialize(ID3D11Device* device,
	int textureWidth, int textureHeight, float screenDepth, float screenNear)
{
	_textureWidth = textureWidth;
	_textureHeight = textureHeight;

	D3D11_TEXTURE2D_DESC textureDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	for (int i = 0; i < BUFFER_COUNT; i++) {
		if (FAILED(device->CreateTexture2D(&textureDesc,
			nullptr, _renderTargetTextureArray[i].GetAddressOf()))) {
			return false;
		}
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;;

	for (int i = 0; i < BUFFER_COUNT; i++) {
		if (FAILED(device->CreateRenderTargetView(_renderTargetTextureArray[i].Get(),
			&renderTargetViewDesc, _renderTargetViewArray[i].GetAddressOf()))) {
			return false;
		}
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < BUFFER_COUNT; i++) {
		if (FAILED(device->CreateShaderResourceView(_renderTargetTextureArray[i].Get(),
			&shaderResourceViewDesc, _shaderResourceViewArray[i].GetAddressOf())))
		{
			return false;
		}
	}

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

	if (FAILED(device->CreateTexture2D(&depthBufferDesc,
		nullptr, _depthStencilBuffer.GetAddressOf()))) {
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateDepthStencilView(_depthStencilBuffer.Get(),
		&depthStencilViewDesc, _depthStencilView.GetAddressOf()))) {
		return false;
	}

	_viewport.Width = (float)textureWidth;
	_viewport.Height = (float)textureHeight;
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;

	return true;
}

void DeferredBuffersClass::SetRenderTargets(ID3D11DeviceContext* deviceContext)
{
	deviceContext->OMSetRenderTargets(BUFFER_COUNT,
		_renderTargetViewArray->GetAddressOf(), _depthStencilView.Get());
	deviceContext->RSSetViewports(1, &_viewport);
}

void DeferredBuffersClass::ClearRenderTargets(ID3D11DeviceContext* deviceContext,
	float red, float green, float blue, float alpha)
{
	float color[4] = { red, green, blue, alpha };

	for (int i = 0; i < BUFFER_COUNT; i++) {
		deviceContext->ClearRenderTargetView(_renderTargetViewArray[i].Get(), color);
	}

	deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);

}

ID3D11ShaderResourceView* DeferredBuffersClass::GetShaderResourceView(int view)
{
	return _shaderResourceViewArray[view].Get();
}
