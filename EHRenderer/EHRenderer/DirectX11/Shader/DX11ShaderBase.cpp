#include "DX11ShaderBase.hpp"
#include "../DX11RE.hpp"
#include <fstream>
#include "../../Data/Transform.hpp"

using namespace std;

void DX11ShaderBase::BindTexture(int slot, int textureId)
{
	ID3D11DeviceContext* deviceContext = DX11RE::GetInstance().GetDeviceContext();
	ID3D11ShaderResourceView* texture = DX11RE::GetInstance().GetTexture(textureId);

	deviceContext->PSSetShaderResources(slot, 1, &texture);
}

void DX11ShaderBase::GetSlot(const std::string& name, int& slotId, size_t& size)
{
}

void DX11ShaderBase::SetSlot(int slotId, int slotOffset, void* valuePtr, size_t size)
{
}

int DX11ShaderBase::GetSlotCnt()
{
	return 0;
}

void DX11ShaderBase::ResetSlot(int slotId, size_t size)
{
}

void DX11ShaderBase::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* filename)
{

	char* compileErrors = (char*)(errorMessage->GetBufferPointer());
	unsigned long long bufferSize = errorMessage->GetBufferSize();

	ofstream fout;
	fout.open("shader-error.txt");

	for (unsigned long long i = 0; i < bufferSize; i++) {
		fout << compileErrors[i];
	}

	fout.close();

	MessageBoxW(hwnd, L"Error compiling shader. Check shader-error.txt for message.", filename, MB_OK);

}

void DX11ShaderBase::GetXMMATRIX(const Transform* position, XMMATRIX& matrix)
{
	if (position == nullptr) {
		matrix = XMMatrixTranslation(0.f, 0.f, 0.f);
		return;
	}

	matrix = XMMatrixTranslation(position->positionX, position->positionY, position->positionZ);
}

bool DX11ShaderBase::CreateConstantBuffer(ID3D11Device* device, unsigned int size, ID3D11Buffer** target)
{
	D3D11_BUFFER_DESC bufferDesc{};

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = size;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	if (FAILED(device->CreateBuffer(&bufferDesc, nullptr, target))) {
		return false;
	}

	return true;

}

bool DX11ShaderBase::CreateSamplerState(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE mode, ID3D11SamplerState** target)
{
	D3D11_SAMPLER_DESC samplerDesc{};

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = mode;
	samplerDesc.AddressV = mode;
	samplerDesc.AddressW = mode;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(device->CreateSamplerState(&samplerDesc, target))) {
		return false;
	}

	return true;
}
