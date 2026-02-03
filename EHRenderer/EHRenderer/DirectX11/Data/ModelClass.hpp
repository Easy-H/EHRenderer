#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <wrl.h>
#include <memory>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

class TextureClass;

class ModelClass {
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};
	struct ModelType {
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};
	struct TempVertexType {
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	struct VectorType {
		float x, y, z;
	};
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(const char* modelFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers();
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadModel(const char*);
	void ReleaseModel();

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);

private:
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;
	int _vertexCount;
	int _indexCount;

	unique_ptr<TextureClass> _texture;
	unique_ptr<ModelType[]> _model;
};