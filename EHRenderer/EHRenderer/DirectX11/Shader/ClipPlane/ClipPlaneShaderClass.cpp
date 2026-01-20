#include "ClipPlaneShaderClass.hpp"
#include <d3dcompiler.h>

ClipPlaneShaderClass::ClipPlaneShaderClass()
{
}

ClipPlaneShaderClass::ClipPlaneShaderClass(const ClipPlaneShaderClass&)
{
}

ClipPlaneShaderClass::~ClipPlaneShaderClass()
{
}

bool ClipPlaneShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	wchar_t vsFilename[128];

	if (wcscpy_s(vsFilename, 128, L"./HLSL/ClipPlane.vs") != 0) {
		return false;
	}

	wchar_t psFilename[128];
	if (wcscpy_s(psFilename, 128, L"./HLSL/ClipPlane.ps") != 0) {
		return false;
	}

	return InitializeShader(device, hwnd, vsFilename, psFilename);
}

bool ClipPlaneShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture, XMFLOAT4 clipPlane) {

	if (!SetShaderParameters(deviceContext,
		worldMatrix, viewMatrix, projectionMatrix, texture, clipPlane)) {
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool ClipPlaneShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) {

	ComPtr<ID3D10Blob> errorMessage;
	ComPtr<ID3D10Blob> vertexShaderBuffer;

	if (FAILED(D3DCompileFromFile(vsFilename, nullptr, nullptr,
		"ClipPlaneVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		vertexShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf()))) {
		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage.Get(), hwnd, vsFilename);
		}
		else {
			MessageBoxW(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	ComPtr<ID3D10Blob> pixelShaderBuffer;

	if (FAILED(D3DCompileFromFile(psFilename, nullptr, nullptr,
		"ClipPlanePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		pixelShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf()))) {
		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage.Get(), hwnd, psFilename);
		}
		else {
			MessageBoxW(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}
		return false;

	}

	if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), nullptr, _vertexShader.GetAddressOf())))
		return false;

	if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), nullptr, _pixelShader.GetAddressOf())))
		return false;

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

	if (FAILED(device->CreateInputLayout(polygonLayout, numElements,
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), _layout.GetAddressOf()))) {
		return false;
	}

	if (!CreateConstantBuffer(device, sizeof(MatrixBufferType),
		_matrixBuffer.GetAddressOf())) {
		return false;
	}

	if (!CreateConstantBuffer(device, sizeof(ClipPlaneBufferType),
		_clipPlaneBuffer.GetAddressOf())) {
		return false;
	}

	if (!CreateSamplerState(device, D3D11_TEXTURE_ADDRESS_WRAP,
		_sampleState.GetAddressOf())) {
		return false;
	}

	return true;

}

bool ClipPlaneShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture, XMFLOAT4 clipPlane) {

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(deviceContext->Map(_matrixBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	MatrixBufferType* ptr = (MatrixBufferType*)mappedResource.pData;

	ptr->worldMatrix = worldMatrix;
	ptr->viewMatrix = viewMatrix;
	ptr->projectionMatrix = projectionMatrix;

	deviceContext->Unmap(_matrixBuffer.Get(), 0);
	deviceContext->VSSetConstantBuffers(0, 1, _matrixBuffer.GetAddressOf());

	if (FAILED(deviceContext->Map(_clipPlaneBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}
	
	ClipPlaneBufferType* ptr2 = (ClipPlaneBufferType*)mappedResource.pData;

	ptr2->clipPlane = clipPlane;

	deviceContext->Unmap(_clipPlaneBuffer.Get(), 0);

	deviceContext->VSSetConstantBuffers(1, 1, _clipPlaneBuffer.GetAddressOf());

	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;

}

void ClipPlaneShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) {

	deviceContext->IASetInputLayout(_layout.Get());

	deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, _sampleState.GetAddressOf());

	deviceContext->DrawIndexed(indexCount, 0, 0);

}