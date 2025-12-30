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

	CalculateModelVectors();

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
		vertices[i].tangent = XMFLOAT3(_model[i].tx, _model[i].ty, _model[i].tz);
		vertices[i].binormal = XMFLOAT3(_model[i].bx, _model[i].by, _model[i].bz);

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

void ModelClass::CalculateModelVectors()
{
	int faceCount = _vertexCount / 3;

	int index = 0;
	for (int i = 0; i < faceCount; i++) {
		TempVertexType vertex[3];

		for (int j = 0; j < 3; j++)
		{
			vertex[j].x = _model[index + j].x;
			vertex[j].y = _model[index + j].y;
			vertex[j].z = _model[index + j].z;
			vertex[j].tu = _model[index + j].tu;
			vertex[j].tv = _model[index + j].tv;
		}

		VectorType tanget, binormal;

		CalculateTangentBinormal(vertex[0], vertex[1], vertex[2], tanget, binormal);

		for (int j = 0; j < 3; j++) {
			_model[index + j].tx = tanget.x;
			_model[index + j].ty = tanget.y;
			_model[index + j].tz = tanget.z;
			_model[index + j].bx = binormal.x;
			_model[index + j].by = binormal.y;
			_model[index + j].bz = binormal.z;
		}

		index += 3;
	}
}


float calcLength(float* arr, int cnt) {

	float sqrRet = 0;
	for (int i = 0; i < cnt; i++) {
		sqrRet += arr[i] * arr[i];
	}
	return sqrt(sqrRet);
}

void ModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
	VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];

	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;
	
	float tanSize[3] = { tangent.x, tangent.y, tangent.z };

	float length = calcLength(tanSize, 3);
	tangent.x /= length;
	tangent.y /= length;
	tangent.z /= length;

	float biSize[3] = { binormal.x, binormal.y, binormal.z };
	length = calcLength(biSize, 3);

	binormal.x /= length;
	binormal.y /= length;
	binormal.z /= length;
}