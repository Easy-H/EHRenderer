#include "DX11Shader.hpp"
#include "../DX11RE.hpp"

static DXGI_FORMAT GetDXGIFormat(D3D11_SIGNATURE_PARAMETER_DESC paramDesc) {
	if (paramDesc.Mask == 1) {
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			return DXGI_FORMAT_R32_UINT;
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			return DXGI_FORMAT_R32_SINT;
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			return DXGI_FORMAT_R32_FLOAT;
	}
	if (paramDesc.Mask <= 3) {
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			return DXGI_FORMAT_R32G32_UINT;
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			return DXGI_FORMAT_R32G32_SINT;
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			return DXGI_FORMAT_R32G32_FLOAT;

	}
	if (paramDesc.Mask <= 7) {
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
			return DXGI_FORMAT_R32G32B32_UINT;
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
			return DXGI_FORMAT_R32G32B32_SINT;
		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
			return DXGI_FORMAT_R32G32B32_FLOAT;

	}
	if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
		return DXGI_FORMAT_R32G32B32A32_UINT;
	if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
		return DXGI_FORMAT_R32G32B32A32_SINT;
	if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
}

bool DX11Shader::Initialize(ID3D11Device* device, HWND hwnd)
{
	return InitializeShader(device, hwnd, _vsFilename, _psFilename);
}

bool DX11Shader::InitializeShader(ID3D11Device* device, HWND hwnd,
	WCHAR* vsFilename, WCHAR* psFilename) {

	ComPtr<ID3D10Blob> errorMessage;
	ComPtr<ID3D10Blob> vertexShaderBuffer;

	if (FAILED(D3DCompileFromFile(vsFilename, nullptr, nullptr,
		_vsFunc, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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
		_psFunc, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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

	ID3D11ShaderReflection* reflection = nullptr;
	D3D11_SHADER_DESC shaderDesc;

	D3DReflect(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		IID_ID3D11ShaderReflection, (void**)&reflection);

	reflection->GetDesc(&shaderDesc);

	SetConstantBuffer(device, reflection, shaderDesc, _vertexSlotStartIdx);

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

	for (UINT i = 0; i < shaderDesc.InputParameters; i++) {
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		reflection->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC elementDesc{};

		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;
		elementDesc.Format = GetDXGIFormat(paramDesc);

		inputLayoutDesc.push_back(elementDesc);
	}

	if (FAILED(device->CreateInputLayout(&inputLayoutDesc[0],
		inputLayoutDesc.size(), vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &_inputLayout))) {
		return false;
	}

	D3DReflect(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
		IID_ID3D11ShaderReflection, (void**)&reflection);

	reflection->GetDesc(&shaderDesc);

	SetConstantBuffer(device, reflection, shaderDesc, _pixelSlotStartIdx);

	if (!CreateSamplerState(device, D3D11_TEXTURE_ADDRESS_WRAP,
		_sampleState.GetAddressOf())) {
		return false;
	}

	return true;
}

bool DX11Shader::SetConstantBuffer(ID3D11Device* device,
	ID3D11ShaderReflection* reflection, D3D11_SHADER_DESC shaderDesc, int& startIdx)
{

	startIdx = _slotData.size();

	for (unsigned int i = 0; i < shaderDesc.ConstantBuffers; i++) {
		ID3D11ShaderReflectionConstantBuffer* buffer = reflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC bufferDesc;

		if (FAILED(buffer->GetDesc(&bufferDesc))) {
			continue;
		}

		ComPtr<ID3D11Buffer> cBuffer;
		CD3D11_BUFFER_DESC cBufferDesc(bufferDesc.Size,
			D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC,
			D3D11_CPU_ACCESS_WRITE);

		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		reflection->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

		if (FAILED(device->CreateBuffer(
			&cBufferDesc, nullptr, cBuffer.GetAddressOf()))) {
			return false;
		}

		std::unique_ptr<SlotInfor> slotInfor = std::make_unique<SlotInfor>();

		slotInfor->slotIdx = bindDesc.BindPoint;
		slotInfor->size = bufferDesc.Size;
		slotInfor->slot = cBuffer;

		_slotData.push_back(std::move(slotInfor));
		
		_slotMap[bufferDesc.Name] = startIdx + i;
	}

	return true;
}

DX11Shader::~DX11Shader()
{
}

bool DX11Shader::Render(int indexCount, const Transform* position)
{
	ID3D11DeviceContext* deviceContext = DX11RE::GetInstance().GetDeviceContext();

	XMMATRIX viewMatrix, projectionMatrix;

	DX11RE::GetInstance().GetView(viewMatrix);
	DX11RE::GetInstance().GetProjection(projectionMatrix);

	XMMATRIX worldMatrix;

	GetXMMATRIX(position, worldMatrix);

	MatrixBufferType matrix = {
		XMMatrixTranspose(worldMatrix),
		XMMatrixTranspose(viewMatrix),
		XMMatrixTranspose(projectionMatrix)
	};

	SetSlot(_matrixBufferSlot, 0, &matrix, sizeof(MatrixBufferType));

	for (int i = _vertexSlotStartIdx; i < _pixelSlotStartIdx; i++) {
		deviceContext->VSSetConstantBuffers(_slotData[i]->slotIdx, 1, _slotData[i]->slot.GetAddressOf());
	}

	for (int i = _pixelSlotStartIdx; i < _slotData.size(); i++) {
		deviceContext->PSSetConstantBuffers(_slotData[i]->slotIdx, 1, _slotData[i]->slot.GetAddressOf());
	}

	deviceContext->IASetInputLayout(_inputLayout.Get());
	deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, _sampleState.GetAddressOf());

	deviceContext->DrawIndexed(indexCount, 0, 0);

	return true;

}

void DX11Shader::SetTargetVS(WCHAR* vsFilename, const char* vsFunc)
{
	_vsFilename = vsFilename;
	_vsFunc = vsFunc;
}

void DX11Shader::SetTargetPS(WCHAR* psFilename, const char* psFunc)
{
	_psFilename = psFilename;
	_psFunc = psFunc;
}

int DX11Shader::GetSlotCnt()
{
	return _slotData.size();
}

void DX11Shader::GetSlot(const std::string& name, int& slotId, size_t& size)
{

	if (!_slotMap.contains(name)) {
		slotId = -1;
		return;
	}

	slotId = _slotMap[name];
	size = _slotData[slotId]->size;

}

void DX11Shader::SetSlot(int slotId, int slotOffset, void* valuePtr, size_t size)
{

	ID3D11DeviceContext* deviceContext = DX11RE::GetInstance().GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(deviceContext->Map(_slotData[slotId]->slot.Get(),
		0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return;
	}

	char* ptr = (char*)mappedResource.pData;

	memcpy(ptr + slotOffset, valuePtr, size);

	deviceContext->Unmap(_slotData[slotId]->slot.Get(), 0);
	_slotData[slotId]->isReset = false;

}

void DX11Shader::ResetSlot(int slotId, size_t size)
{
	if (slotId == _matrixBufferSlot) return;

	ID3D11DeviceContext* deviceContext = DX11RE::GetInstance().GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(deviceContext->Map(_slotData[slotId]->slot.Get(),
		0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return;
	}

	char* ptr = (char*)mappedResource.pData;

	ZeroMemory(ptr, size);

	deviceContext->Unmap(_slotData[slotId]->slot.Get(), 0);
	_slotData[slotId]->isReset = true;

}