#include "ProjectionShaderClass.hpp"

#include <d3dcompiler.h>

ProjectionShaderClass::ProjectionShaderClass()
{
}

ProjectionShaderClass::ProjectionShaderClass(const ProjectionShaderClass&)
{
}

ProjectionShaderClass::~ProjectionShaderClass()
{
}


bool ProjectionShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	wchar_t vsFilename[128];
	if (wcscpy_s(vsFilename, 128, L"./HLSL/Projection.vs") != 0) return false;

	wchar_t psFilename[128];
	if (wcscpy_s(psFilename, 128, L"./HLSL/Projection.ps") != 0) return false;

	return InitializeShader(device, hwnd, vsFilename, psFilename);
}

void ProjectionShaderClass::Shutdown() {
	ShutdownShader();
}

bool ProjectionShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	XMMATRIX viewMatrix2, XMMATRIX projectionMatrix2,
	ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* projectionTexture,
	XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightPosition, float brightness)
{

	if (!SetShaderParameters(deviceContext,
		worldMatrix, viewMatrix, projectionMatrix,
		viewMatrix2, projectionMatrix2,
		texture, projectionTexture,
		ambientColor, diffuseColor, lightPosition, brightness)) return false;

	RenderShader(deviceContext, indexCount);
	return true;
}

bool ProjectionShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	ComPtr<ID3D10Blob> errorMessage;

	ComPtr<ID3D10Blob> vertexShaderBuffer;

	if (FAILED(D3DCompileFromFile(vsFilename, nullptr, nullptr,
		"ProjectionVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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
		"ProjectionPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

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

	if (!CreateConstantBuffer(device, sizeof(LightPositionBufferType),
		_lightPositionBuffer.GetAddressOf())) {
		return false;
	}

	if (!CreateConstantBuffer(device, sizeof(LightBufferType),
		_lightBuffer.GetAddressOf())) {
		return false;
	}

	if (!CreateSamplerState(device, D3D11_TEXTURE_ADDRESS_WRAP,
		_sampleState.GetAddressOf())) {
		return false;
	}

	return true;

}

void ProjectionShaderClass::ShutdownShader()
{
}

bool ProjectionShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	XMMATRIX viewMatrix2, XMMATRIX projectionMatrix2,
	ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* projectionTexture,
	XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightPosition, float brightness)
{

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	viewMatrix2 = XMMatrixTranspose(viewMatrix2);
	projectionMatrix2 = XMMatrixTranspose(projectionMatrix2);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(deviceContext->Map(_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	dataPtr->view2 = viewMatrix2;
	dataPtr->projection2 = projectionMatrix2;

	deviceContext->Unmap(_matrixBuffer.Get(), 0);

	unsigned int bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, _matrixBuffer.GetAddressOf());
	
	if (FAILED(deviceContext->Map(_lightPositionBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	LightPositionBufferType* dataPtr2 = (LightPositionBufferType*)mappedResource.pData;

	dataPtr2->lightPosition = lightPosition;

	deviceContext->Unmap(_lightPositionBuffer.Get(), 0);

	deviceContext->VSSetConstantBuffers(1, 1, _lightPositionBuffer.GetAddressOf());

	if (FAILED(deviceContext->Map(_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	LightBufferType* dataPtr3 = (LightBufferType*)mappedResource.pData;

	dataPtr3->ambientColor = ambientColor;
	dataPtr3->brightness = brightness;
	dataPtr3->diffuseColor = diffuseColor;

	deviceContext->Unmap(_lightBuffer.Get(), 0);

	deviceContext->PSSetConstantBuffers(0, 1, _lightBuffer.GetAddressOf());

	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &projectionTexture);

	return true;
}

void ProjectionShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) {
	deviceContext->IASetInputLayout(_layout.Get());
	deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, _sampleState.GetAddressOf());
	deviceContext->DrawIndexed(indexCount, 0, 0);
}