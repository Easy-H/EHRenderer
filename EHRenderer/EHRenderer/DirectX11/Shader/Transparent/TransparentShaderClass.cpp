#include "TransparentShaderClass.hpp"

#include <d3dcompiler.h>

TransparentShaderClass::TransparentShaderClass()
{
}

TransparentShaderClass::TransparentShaderClass(const TransparentShaderClass&)
{
}

TransparentShaderClass::~TransparentShaderClass()
{
}


bool TransparentShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	wchar_t vsFilename[128];
	if (wcscpy_s(vsFilename, 128, L"./HLSL/Transparent.vs") != 0) return false;

	wchar_t psFilename[128];
	if (wcscpy_s(psFilename, 128, L"./HLSL/Transparent.ps") != 0) return false;

	return InitializeShader(device, hwnd, vsFilename, psFilename);
}

void TransparentShaderClass::Shutdown() {
	ShutdownShader();
}

bool TransparentShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* Transparent, float blend)
{
	if (!SetShaderParameters(deviceContext,
		worldMatrix, viewMatrix, projectionMatrix, Transparent, blend)) return false;

	RenderShader(deviceContext, indexCount);
	return true;
}

bool TransparentShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	ComPtr<ID3D10Blob> errorMessage;

	ComPtr<ID3D10Blob> vertexShaderBuffer;

	if (FAILED(D3DCompileFromFile(vsFilename, nullptr, nullptr,
		"TransparentVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		vertexShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf()))) {
		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage.Get(), hwnd, vsFilename);
		}
		else {
			MessageBoxW(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	if (FAILED(device->CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, _vertexShader.GetAddressOf()))) {
		return false;
	}

	ComPtr<ID3D10Blob> pixelShaderBuffer;

	if (FAILED(D3DCompileFromFile(psFilename, nullptr, nullptr,
		"TransparentPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		pixelShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf()))) {
		if (errorMessage) {
			OutputShaderErrorMessage(errorMessage.Get(), hwnd, psFilename);
		}
		else {
			MessageBoxW(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	if (FAILED(device->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, _pixelShader.GetAddressOf()))) {
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

	if (FAILED(device->CreateInputLayout(polygonLayout, numElements,
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		_layout.GetAddressOf()))) {
		return false;
	}

	if (!CreateConstantBuffer(device, sizeof(MatrixBufferType),
		_matrixBuffer.GetAddressOf())) {
		return false;
	}

	if (!CreateConstantBuffer(device, sizeof(TransparentBufferType),
		_transparentBuffer.GetAddressOf())) {
		return false;
	}

	if (!CreateSamplerState(device, D3D11_TEXTURE_ADDRESS_WRAP,
		_sampleState.GetAddressOf())) {
		return false;
	}

	return true;

}

void TransparentShaderClass::ShutdownShader()
{
}

bool TransparentShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture, float blend) {

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(deviceContext->Map(_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(_matrixBuffer.Get(), 0);

	deviceContext->VSSetConstantBuffers(0, 1, _matrixBuffer.GetAddressOf());

	if (FAILED(deviceContext->Map(_transparentBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	TransparentBufferType* dataPtr2 = (TransparentBufferType*)mappedResource.pData;

	dataPtr2->blendAmount = blend;

	deviceContext->Unmap(_transparentBuffer.Get(), 0);

	deviceContext->PSSetConstantBuffers(0, 1,_transparentBuffer.GetAddressOf());
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void TransparentShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) {
	deviceContext->IASetInputLayout(_layout.Get());
	deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, _sampleState.GetAddressOf());
	deviceContext->DrawIndexed(indexCount, 0, 0);
}