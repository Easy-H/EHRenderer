#include "TextClass.hpp"

#include <memory>
#include "../Font/FontClass.hpp"

using namespace std;

TextClass::TextClass()
{
}

TextClass::TextClass(const TextClass&)
{
}

TextClass::~TextClass()
{
}

bool TextClass::TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	int screenWidth, int screenHeight, int maxLength, FontClass* font, char* text,
	int positionX, int positionY, float red, float green, float blue)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_maxLength = maxLength;


	if (!InitializeBuffers(device, deviceContext, font, text, positionX, positionY, red, green, blue))
		return false;

	return true;
}

void TextClass::Shutdown() {}
void TextClass::Render(ID3D11DeviceContext* deviceContext) {
	RenderBuffers(deviceContext);
}

int TextClass::GetIndexCount() {
	return  _indexCount;
}

bool TextClass::InitializeBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	FontClass* font, char* text, int positionX, int positionY, float red, float green, float blue) {

	_vertexCount = 6 * _maxLength;
	_indexCount = _vertexCount;

	unique_ptr<VertexType[]> vertices = make_unique<VertexType[]>(_vertexCount);
	unique_ptr<unsigned long[]> indices = make_unique<unsigned long[]>(_indexCount);

	memset(vertices.get(), 0, (sizeof(VertexType) * _vertexCount));

	for (int i = 0; i < _indexCount; i++) {
		indices[i] = i;
	}


	D3D11_BUFFER_DESC vertexBufferDesc{};

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData{};

	vertexData.pSysMem = vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, _vertexBuffer.GetAddressOf())))
		return false;

	D3D11_BUFFER_DESC indexBufferDesc{};

	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData{};

	indexData.pSysMem = indices.get();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, _indexBuffer.GetAddressOf())))
		return false;

	return UpdateText(deviceContext, font, text, positionX, positionY, red, green, blue);

}

bool TextClass::UpdateText(ID3D11DeviceContext* deviceContext,
	FontClass* font, char* text, int positionX, int positionY, float red, float green, float blue) {

	_pixelColor = XMFLOAT4(red, green, blue, 1.0f);

	int numLetters = (int)strlen(text);

	if (numLetters > _maxLength) return false;

	unique_ptr<VertexType[]> vertices = make_unique<VertexType[]>(_vertexCount);
	memset(vertices.get(), 0, (sizeof(VertexType) * _vertexCount));

	float drawX = (float)(((_screenWidth / 2) * -1) + positionX);
	float drawY = (float)((_screenHeight / 2) - positionY);

	font->BuildVertexArray((void*)vertices.get(), text, drawX, drawY);

	D3D11_MAPPED_SUBRESOURCE mappedResource{};

	if (FAILED(deviceContext->Map(_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	VertexType* verticesPtr = (VertexType*)mappedResource.pData;

	memcpy(verticesPtr, (void*)vertices.get(), (sizeof(VertexType) * _vertexCount));
	deviceContext->Unmap(_vertexBuffer.Get(), 0);
	
	return true;

}
void TextClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

XMFLOAT4 TextClass::GetPixelColor() {
	return _pixelColor;
}
void TextClass::ShutdownBuffers() {}