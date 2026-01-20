#include "FireShaderClass.hpp"

#include <d3dcompiler.h>

FireShaderClass::FireShaderClass()
{
}

FireShaderClass::FireShaderClass(const FireShaderClass&)
{
}

FireShaderClass::~FireShaderClass()
{
}


bool FireShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	wchar_t vsFilename[128];
	if (wcscpy_s(vsFilename, 128, L"./HLSL/Fire.vs") != 0) return false;

	wchar_t psFilename[128];
	if (wcscpy_s(psFilename, 128, L"./HLSL/Fire.ps") != 0) return false;

	return InitializeShader(device, hwnd, vsFilename, psFilename);
}

void FireShaderClass::Shutdown() {
	ShutdownShader();
}

bool FireShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* fireTexture, ID3D11ShaderResourceView* noiseTexture,
	ID3D11ShaderResourceView* alphaTexture, float frameTime,
	XMFLOAT3 scrollSpeeds, XMFLOAT3 scales,
	XMFLOAT2 distortion1, XMFLOAT2 distortion2, XMFLOAT2 distortion3,
	float distortionScale, float distortionBias)
{
	if (!SetShaderParameters(deviceContext,
		worldMatrix, viewMatrix, projectionMatrix, fireTexture, noiseTexture,
		alphaTexture, frameTime, scrollSpeeds, scales, distortion1, distortion2, distortion3, 
		distortionScale, distortionBias)) return false;

	RenderShader(deviceContext, indexCount);
	return true;
}

bool FireShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	ComPtr<ID3D10Blob> errorMessage;

	ComPtr<ID3D10Blob> vertexShaderBuffer;

	if (FAILED(D3DCompileFromFile(vsFilename, nullptr, nullptr,
		"FireVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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
		"FirePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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

	if (!CreateConstantBuffer(device, sizeof(DistortionBufferType),
		_distortionBuffer.GetAddressOf())) {
		return false;
	}

	if (!CreateConstantBuffer(device, sizeof(NoiseBufferType),
		_noiseBuffer.GetAddressOf())) {
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

void FireShaderClass::ShutdownShader()
{
}

bool FireShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* fireTexture, ID3D11ShaderResourceView* noiseTexture,
	ID3D11ShaderResourceView* alphaTexture, float frameTime,
	XMFLOAT3 scrollSpeeds, XMFLOAT3 scales,
	XMFLOAT2 distortion1, XMFLOAT2 distortion2, XMFLOAT2 distortion3,
	float distortionScale, float distortionBias) {

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(deviceContext->Map(_matrixBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(_matrixBuffer.Get(), 0);

	deviceContext->VSSetConstantBuffers(0, 1, _matrixBuffer.GetAddressOf());

	if (FAILED(deviceContext->Map(_noiseBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	NoiseBufferType* dataPtr2 = (NoiseBufferType*)mappedResource.pData;

	dataPtr2->frameTime = frameTime;
	dataPtr2->scales = scales;
	dataPtr2->scrollSpeeds = scrollSpeeds;

	deviceContext->Unmap(_noiseBuffer.Get(), 0);

	deviceContext->VSSetConstantBuffers(1, 1, _noiseBuffer.GetAddressOf());

	if (FAILED(deviceContext->Map(_distortionBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	DistortionBufferType* dataPtr3 = (DistortionBufferType*)mappedResource.pData;

	dataPtr3->distortion1 = distortion1;
	dataPtr3->distortion2 = distortion2;
	dataPtr3->distortion3 = distortion3;
	dataPtr3->distortionBias = distortionBias;
	dataPtr3->distortionScale = distortionScale;

	deviceContext->Unmap(_distortionBuffer.Get(), 0);

	deviceContext->PSSetConstantBuffers(0, 1, _distortionBuffer.GetAddressOf());

	deviceContext->PSSetShaderResources(0, 1, &fireTexture);
	deviceContext->PSSetShaderResources(1, 1, &noiseTexture);
	deviceContext->PSSetShaderResources(2, 1, &alphaTexture);

	return true;
}

void FireShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) {
	deviceContext->IASetInputLayout(_layout.Get());
	deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, _sampleStateWrap.GetAddressOf());
	deviceContext->PSSetSamplers(1, 1, _sampleStateClamp.GetAddressOf());

	deviceContext->DrawIndexed(indexCount, 0, 0);
}