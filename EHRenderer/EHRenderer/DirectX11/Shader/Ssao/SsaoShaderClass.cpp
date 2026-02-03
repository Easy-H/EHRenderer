#include "SsaoShaderClass.hpp"

#include <d3dcompiler.h>

SsaoShaderClass::SsaoShaderClass()
{
}

SsaoShaderClass::SsaoShaderClass(const SsaoShaderClass&)
{
}

SsaoShaderClass::~SsaoShaderClass()
{
}

bool SsaoShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	wchar_t vsFilename[128];

	if (wcscpy_s(vsFilename, 128, L"./HLSL/Ssao/Ssao.vs") != 0) return false;

	wchar_t psFilename[128];
	if (wcscpy_s(psFilename, 128, L"./HLSL/Ssao/Ssao.ps") != 0) return false;

	return InitializeShader(device, hwnd, vsFilename, psFilename);
}

bool SsaoShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* positionTexture, ID3D11ShaderResourceView* normalTexture,
	ID3D11ShaderResourceView* randomTexture,
	float screenWidth, float screenHeight, float randomTextureSize, float sampleRadius,
	float ssaoScale, float ssaoBias, float ssaoIntensity)
{
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix,
		positionTexture, normalTexture, randomTexture,
		screenWidth, screenHeight, randomTextureSize, sampleRadius,
		ssaoScale, ssaoBias, ssaoIntensity)) return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

bool SsaoShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	ComPtr<ID3D10Blob> errorMessage;
	ComPtr<ID3D10Blob> vertexShaderBuffer;

	if (FAILED(D3DCompileFromFile(vsFilename, nullptr, nullptr, "SsaoVertexShader", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, vertexShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf()))) {

		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage.Get(), hwnd, vsFilename);
		}
		else {
			MessageBoxW(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	ComPtr<ID3D10Blob> pixelShaderBuffer;

	if (FAILED(D3DCompileFromFile(psFilename, nullptr, nullptr, "SsaoPixelShader", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, pixelShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf()))) {

		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage.Get(), hwnd, psFilename);
		}
		else {
			MessageBoxW(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		0, _vertexShader.GetAddressOf()))) {
		return false;
	}

	if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
		0, _pixelShader.GetAddressOf()))) {
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	if (FAILED(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), _inputLayout.GetAddressOf()))) {
		return false;
	}

	if (!CreateConstantBuffer(device, sizeof(MatrixBufferType),
		_matrixBuffer.GetAddressOf())) {
		return false;
	}

	if (!CreateConstantBuffer(device, sizeof(SsaoBufferType),
		_ssaoBuffer.GetAddressOf())) {
		return false;
	}

	if (!CreateSamplerState(device, D3D11_TEXTURE_ADDRESS_WRAP,
		_sampleStateWrap.GetAddressOf())) {
		return false;
	}

	if (!CreateSamplerState(device, D3D11_TEXTURE_ADDRESS_CLAMP,
		_sampleStateClamp.GetAddressOf())) {
		return false;
	}


	return true;
}

bool SsaoShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* positionTexture, ID3D11ShaderResourceView* normalTexture,
	ID3D11ShaderResourceView* randomTexture,
	float screenWidth, float screenHeight, float randomTextureSize,
	float sampleRadius, float ssaoScale, float ssaoBias, float ssaoIntensity)
{
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(deviceContext->Map(_matrixBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	MatrixBufferType* ptr1 = (MatrixBufferType*)mappedResource.pData;
	ptr1->worldMatrix = worldMatrix;
	ptr1->viewMatrix = viewMatrix;
	ptr1->projectionMatrix = projectionMatrix;

	deviceContext->Unmap(_matrixBuffer.Get(), 0);

	deviceContext->VSSetConstantBuffers(0, 1, _matrixBuffer.GetAddressOf());

	if (FAILED(deviceContext->Map(_ssaoBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	SsaoBufferType* ptr2 = (SsaoBufferType*)mappedResource.pData;

	ptr2->screenWidth = screenWidth;
	ptr2->screenHeight = screenHeight;
	ptr2->randomTextureSize = randomTextureSize;
	ptr2->sampleRadius = sampleRadius;
	ptr2->ssaoScale = ssaoScale;
	ptr2->ssaoBias = ssaoBias;
	ptr2->ssaoIntensity = ssaoIntensity;
	ptr2->padding = 0.f;

	deviceContext->Unmap(_ssaoBuffer.Get(), 0);

	deviceContext->PSSetConstantBuffers(0, 1, _ssaoBuffer.GetAddressOf());

	deviceContext->PSGetShaderResources(0, 1, &positionTexture);
	deviceContext->PSGetShaderResources(1, 1, &normalTexture);
	deviceContext->PSGetShaderResources(2, 1, &randomTexture);

	return true;
}

void SsaoShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) {

	deviceContext->IASetInputLayout(_inputLayout.Get());

	deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, _sampleStateWrap.GetAddressOf());
	deviceContext->PSSetSamplers(1, 1, _sampleStateClamp.GetAddressOf());

	deviceContext->DrawIndexed(indexCount, 0, 0);

}