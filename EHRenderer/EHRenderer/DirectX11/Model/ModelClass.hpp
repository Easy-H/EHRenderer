#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;
class ModelClass {
private:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;
	int _vertexCount;
	int _indexCount;



};