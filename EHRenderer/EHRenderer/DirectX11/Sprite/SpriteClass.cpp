#include "SpriteClass.hpp"
#include "../Shader/TextureClass.hpp"

SpriteClass::SpriteClass()
{
}

SpriteClass::SpriteClass(const SpriteClass&)
{
}

SpriteClass::~SpriteClass()
{
}

bool SpriteClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	int screenWidth, int screenHeight, char* textureFilename, int renderX, int renderY)
{

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_renderX = renderX;
	_renderY = renderY;

	if (!InitializeBuffers(device)) return false;
	if (!LoadTexture(device, deviceContext, textureFilename)) return false;

	return true;
}

void SpriteClass::Shutdown() {
	ReleaseTexture();
	ShutdownBuffers();
}

bool SpriteClass::Render(ID3D11DeviceContext* deviceContext)
{
	if (!UpdateBuffers(deviceContext)) return false;

	RenderBuffers(deviceContext);
	return true;
}

int SpriteClass::GetIndexCount() {
	return _indexCount;
}

ID3D11ShaderResourceView* SpriteClass::GetTexture() {
	return _texture->GetTexture();
}

void SpriteClass::SetRenderLocation(int x, int y)
{
	_renderX = x;
	_renderY = y;
}

bool SpriteClass::InitializeBuffers(ID3D11Device* device)
{
	_prevPosX = -1;
	_prevPosY = -1;

	_vertexCount = 6;
	_indexCount = _vertexCount;
	
	unique_ptr<VertexType[]> vertices = make_unique<VertexType[]>(_vertexCount);
	unique_ptr<long[]> indices = make_unique<long[]>(_indexCount);

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

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, _vertexBuffer.GetAddressOf()))) {
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

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, _indexBuffer.GetAddressOf()))) {
		return false;
	}
	return true;
}

void SpriteClass::ShutdownBuffers() {

}

bool SpriteClass::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	
	if (_prevPosX == _renderX && _prevPosY == _renderY) return true;

	_prevPosX = _renderX;
	_prevPosY = _renderY;

	unique_ptr<VertexType[]> vertices = make_unique<VertexType[]>(_vertexCount);

	float left = (float)((_screenWidth / 2) * -1) + (float)_renderX;
	float right = left + (float)_spriteWidth;
	float top = (float)(_screenHeight / 2) + (float)_renderY;
	float bottom = top - (float)_spriteHeight;

	vertices[0].position = XMFLOAT3(left, top, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);
	
	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);
	
	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);
	
	vertices[3].position = XMFLOAT3(left, top, 0.0f);
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(deviceContext->Map(_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		 return false;

	VertexType* dataPtr = (VertexType*)mappedResource.pData;
	memcpy(dataPtr, (void*)vertices.get(), (sizeof(VertexType) * _vertexCount));

	deviceContext->Unmap(_vertexBuffer.Get(), 0);
	dataPtr = 0;

	return true;
}


bool SpriteClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{

	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

bool SpriteClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{

	_texture = make_unique<TextureClass>();
	if (!_texture->Initialize(device, deviceContext, filename)) return false;

	_spriteWidth = _texture->GetWidth();
	_spriteHeight = _texture->GetHeight();

	return true;
}

void SpriteClass::ReleaseTexture() {

}