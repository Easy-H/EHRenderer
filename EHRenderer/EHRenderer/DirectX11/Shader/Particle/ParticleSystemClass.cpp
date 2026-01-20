#include "ParticleSystemClass.hpp"
#include "../../Data/TextureClass.hpp"

ParticleSystemClass::ParticleSystemClass()
{
}

ParticleSystemClass::ParticleSystemClass(const ParticleSystemClass&)
{
}

ParticleSystemClass::~ParticleSystemClass()
{
}

bool ParticleSystemClass::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* deviceContext, char* textureFilename) {

	if (!LoadTexture(device, deviceContext, textureFilename)) {
		return false;
	}

	if (!InitializeParticleSystem()) {
		return false;
	}

	if (!InitializeBuffers(device)) {
		return false;
	}

	return true;

}


bool ParticleSystemClass::Frame(float frameTime, ID3D11DeviceContext* deviceContext)
{
	KillParticles();

	EmitParticles(frameTime);

	UpdateParticles(frameTime);

	if (!UpdateBuffers(deviceContext)) {
		return false;
	}

	return true;
}

void ParticleSystemClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

ID3D11ShaderResourceView* ParticleSystemClass::GetTexture() {
	return _texture->GetTexture();
}

int ParticleSystemClass::GetIndexCount() {
	return _indexCount;
}


bool ParticleSystemClass::LoadTexture(ID3D11Device* device,
	ID3D11DeviceContext* deviceContext, char* filename) {

	_texture = make_unique<TextureClass>();

	if (!_texture->Initialize(device, deviceContext, filename)) {
		return false;
	}

	return true;
}

bool ParticleSystemClass::InitializeParticleSystem()
{
	_particleDeviationX = 0.5f;
	_particleDeviationY = 0.1f;
	_particleDeviationZ = 2.f;

	_particleVelocity = 1.f;
	_particleVelocityVariation = 0.2f;

	_particleSize = 0.2f;

	_particlePerSecond = 100.f;

	_maxParticles = 1000;

	_particleList = make_unique<ParticleType[]>(_maxParticles);

	for (int i = 0; i < _maxParticles; i++) {
		_particleList[i].active = false;
	}

	_currentParticleCount = 0;
	_accumulatedTime = 0.f;

	return true;
}

bool ParticleSystemClass::InitializeBuffers(ID3D11Device* device) {

	_vertexCount = _maxParticles * 6;
	_indexCount = _vertexCount;

	_vertices = make_unique<VertexType[]>(_vertexCount);
	unique_ptr<unsigned long[]> indices = make_unique<unsigned long[]>(_indexCount);

	memset(_vertices.get(), 0, (sizeof(VertexType) * _vertexCount));

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

	vertexData.pSysMem = _vertices.get();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData,
		_vertexBuffer.GetAddressOf()))) {
		return false;
	}


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

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData,
		_indexBuffer.GetAddressOf()))) {
		return false;
	}

	return true;
	
}

void ParticleSystemClass::RenderBuffers(ID3D11DeviceContext* deviceContext) {
	
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, 
		_vertexBuffer.GetAddressOf(), &stride, &offset);

	deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ParticleSystemClass::EmitParticles(float frameTime)
{
	_accumulatedTime += frameTime;
	bool emitParticle = false;

	if (_accumulatedTime > (1.f / _particlePerSecond)) {
		_accumulatedTime = 0.0f;
		emitParticle = true;
	}

	if (!emitParticle || (_currentParticleCount >= (_maxParticles - 1))) {
		return;
	}

	_currentParticleCount++;

	float positionX = (((float)rand() - (float)rand()) / RAND_MAX) * _particleDeviationX;
	float positionY = (((float)rand() - (float)rand()) / RAND_MAX) * _particleDeviationY;
	float positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * _particleDeviationZ;
	
	float velocity = _particleVelocity + (((float)rand()) / RAND_MAX) * _particleVelocityVariation;

	float red = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
	float green = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
	float blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

	int index = 0;

	bool found = false;

	while (!found) {
		if ((_particleList[index].active == false || (_particleList[index].positionZ < positionZ))) {
			found = true;
		}
		else {
			index++;
		}
	}

	int i = _currentParticleCount;

	while (i != index) {
		_particleList[i].positionX = _particleList[i - 1].positionX;
		_particleList[i].positionY = _particleList[i - 1].positionY;
		_particleList[i].positionZ = _particleList[i - 1].positionZ;
		_particleList[i].red = _particleList[i - 1].red;
		_particleList[i].green = _particleList[i - 1].green;
		_particleList[i].blue = _particleList[i - 1].blue;
		_particleList[i].velocity = _particleList[i - 1].velocity;
		_particleList[i].active = _particleList[i - 1].active;

		i--;
	}

	_particleList[i].positionX = positionX;
	_particleList[i].positionY = positionY;
	_particleList[i].positionZ = positionZ;
	_particleList[i].red = red;
	_particleList[i].green =green;
	_particleList[i].blue = blue;
	_particleList[i].velocity = velocity;
	_particleList[i].active = true;

}

void ParticleSystemClass::UpdateParticles(float frameTime)
{
	for (int i = 0; i < _currentParticleCount; i++) {
		_particleList[i].positionY =
			_particleList[i].positionY - (_particleList[i].velocity * frameTime);
	}

}

void ParticleSystemClass::KillParticles()
{
	for (int i = 0; i < _maxParticles; i++) {
		if (!_particleList[i].active || _particleList[i].positionY >= -3.f) {
			continue;
		}

		_particleList[i].active = false;
		_currentParticleCount--;

		for (int j = 0; j < _maxParticles - 1; j++) {
			_particleList[j].positionX = _particleList[j + 1].positionX;
			_particleList[j].positionY = _particleList[j + 1].positionY;
			_particleList[j].positionZ = _particleList[j + 1].positionZ;
			_particleList[j].red = _particleList[j + 1].red;
			_particleList[j].green = _particleList[j + 1].green;
			_particleList[j].blue = _particleList[j + 1].blue;
			_particleList[j].velocity = _particleList[j + 1].velocity;
			_particleList[j].active = _particleList[j + 1].active;
		}
	}
}


bool ParticleSystemClass::UpdateBuffers(ID3D11DeviceContext* deviceContext) {

	memset(_vertices.get(), 0, (sizeof(VertexType) * _vertexCount));

	int index = 0;

	for (int i = 0; i < _currentParticleCount; i++) {

		for (int j = 0; j < 6; j++)
		{
			_vertices[j + index].color = XMFLOAT4(
				_particleList[i].red,
				_particleList[i].green,
				_particleList[i].blue,
				1.f);
		}

		_vertices[index].position = XMFLOAT3(
			_particleList[i].positionX - _particleSize,
			_particleList[i].positionY - _particleSize,
			_particleList[i].positionZ);
		_vertices[index].texture = XMFLOAT2(0.f, 1.f);

		index++;

		_vertices[index].position = XMFLOAT3(
			_particleList[i].positionX - _particleSize,
			_particleList[i].positionY + _particleSize,
			_particleList[i].positionZ);
		_vertices[index].texture = XMFLOAT2(0.f, 0.f);

		index++;

		_vertices[index].position = XMFLOAT3(
			_particleList[i].positionX + _particleSize,
			_particleList[i].positionY - _particleSize,
			_particleList[i].positionZ);
		_vertices[index].texture = XMFLOAT2(1.f, 1.f);

		index++;

		_vertices[index].position = XMFLOAT3(
			_particleList[i].positionX + _particleSize,
			_particleList[i].positionY - _particleSize,
			_particleList[i].positionZ);
		_vertices[index].texture = XMFLOAT2(1.f, 1.f);

		index++;

		_vertices[index].position = XMFLOAT3(
			_particleList[i].positionX - _particleSize,
			_particleList[i].positionY + _particleSize,
			_particleList[i].positionZ);
		_vertices[index].texture = XMFLOAT2(0.f, 0.f);

		index++;

		_vertices[index].position = XMFLOAT3(
			_particleList[i].positionX + _particleSize,
			_particleList[i].positionY + _particleSize,
			_particleList[i].positionZ);
		_vertices[index].texture = XMFLOAT2(1.f, 0.f);

		index++;

	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if (FAILED(deviceContext->Map(_vertexBuffer.Get(), 0,
		D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		return false;
	}

	VertexType* verticesPtr = (VertexType*)(mappedResource.pData);

	memcpy(verticesPtr, (void*)_vertices.get(), (sizeof(VertexType) * _vertexCount));

	deviceContext->Unmap(_vertexBuffer.Get(), 0);
	return true;
}