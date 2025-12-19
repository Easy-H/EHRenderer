#include "ModelClass.hpp"
#include "TextureClass.hpp"

ModelClass::ModelClass()
{

}

ModelClass::ModelClass(const ModelClass&)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	char* modelFilename, char* textureFilename)
{
	if (!LoadModel(modelFilename)) return false;
	if (!InitializeBuffers(device)) return false;

	return LoadTexture(device, deviceContext, textureFilename);
}

void ModelClass::Shutdown() {
	ReleaseTexture();
	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
	return _indexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{

	VertexType* vertices = new VertexType[_vertexCount];
	if (!vertices) return false;

	unsigned long* indices = new unsigned long[_indexCount];
	if (!indices) return false;

	for (int i = 0; i < _vertexCount; i++) {
		vertices[i].position = XMFLOAT3(_model[i].x, _model[i].y, _model[i].z);
		vertices[i].texture = XMFLOAT2(_model[i].tu, _model[i].tv);
		vertices[i].normal = XMFLOAT3(_model[i].nx, _model[i].ny, _model[i].nz);

		indices[i] = i;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData,
		_vertexBuffer.GetAddressOf())))
		return false;

	D3D11_BUFFER_DESC indexBufferDesc{};
	D3D11_SUBRESOURCE_DATA indexData{};

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData,
		_indexBuffer.GetAddressOf()))) {
		return false;
	}

	delete[] vertices;
	delete[] indices;

	return true;
}

void ModelClass::ShutdownBuffers()
{

}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

ID3D11ShaderResourceView* ModelClass::GetTexture() {
	return _texture->GetTexture();
}

bool ModelClass::LoadTexture(ID3D11Device* device,
	ID3D11DeviceContext* deviceContext, char* filename)
{
	_texture = std::make_unique<TextureClass>();
	if (!_texture->Initialize(device, deviceContext, filename)) {
		return false;
	}
	return true;
}

void ModelClass::ReleaseTexture()
{
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	fin.open(filename);

	if (fin.fail()) return false;

	fin.get(input);
	while (input != ':') {
		fin.get(input);
	}

	fin >> _vertexCount;
	_indexCount = _vertexCount;

	_model = make_unique<ModelType[]>(_vertexCount);

	fin.get(input);
	while (input != ':') {
		fin.get(input);
	}

	fin.get(input);
	fin.get(input);

	for (int i = 0; i < _vertexCount; i++) {
		fin >> _model[i].x >> _model[i].y >> _model[i].z;
		fin >> _model[i].tu >> _model[i].tv;
		fin >> _model[i].nx >> _model[i].ny >> _model[i].nz;
	}

	fin.close();


	return true;
}

void ModelClass::ReleaseModel()
{
}
