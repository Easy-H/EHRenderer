#include "OrthoWindowClass.hpp"
#include <memory>

using namespace std;

OrthoWindowClass::OrthoWindowClass()
{
}

OrthoWindowClass::OrthoWindowClass(const OrthoWindowClass&)
{
}

OrthoWindowClass::~OrthoWindowClass()
{
}

bool OrthoWindowClass::Initialize(ID3D11Device* device, int windowWidth, int windowHeight)
{
	if (!InitializeBuffers(device, windowWidth, windowHeight)) {
		return false;
	}
	return true;
}
void OrthoWindowClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int OrthoWindowClass::GetIndexCount()
{
	return _indexCount;
}

bool OrthoWindowClass::InitializeBuffers(ID3D11Device* device,
	int windowWidth, int windowHeight)
{
	float left = (float)((windowWidth / 2) * -1);
	float right = left + (float)windowWidth;

	float top = (float)(windowHeight / 2);
	float bottom = top - (float)windowHeight;

	_vertexCount = 6;
	_indexCount = _vertexCount;

	unique_ptr<VertexType[]> vertices = make_unique<VertexType[]>(_vertexCount);
	unique_ptr<unsigned long[]> indices = make_unique<unsigned long[]>(_indexCount);

	vertices[0].position = XMFLOAT3(left, top, 0.f);
	vertices[0].texture = XMFLOAT2(0.f, 0.f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.f);
	vertices[1].texture = XMFLOAT2(1.f, 1.f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.f);
	vertices[2].texture = XMFLOAT2(0.f, 1.f);

	vertices[3].position = XMFLOAT3(left, top, 0.f);
	vertices[3].texture = XMFLOAT2(0.f, 0.f);

	vertices[4].position = XMFLOAT3(right, top, 0.f);
	vertices[4].texture = XMFLOAT2(1.f, 0.f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.f);
	vertices[5].texture = XMFLOAT2(1.f, 1.f);

	for (int i = 0; i < _indexCount; i++) {
		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc{};

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData{};

	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData,
		_vertexBuffer.GetAddressOf()))) {
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc{};

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData{};

	indexData.pSysMem = indices.get();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData,
		_indexBuffer.GetAddressOf()))) {
		return false;
	}

	return true;
}

void OrthoWindowClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1,
		_vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer.Get(),
		DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}