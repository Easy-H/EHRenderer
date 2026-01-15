#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class OrthoWindowClass {
private:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
public:
	OrthoWindowClass();
	OrthoWindowClass(const OrthoWindowClass&);
	~OrthoWindowClass();

	bool Initialize(ID3D11Device*, int, int);
	void Render(ID3D11DeviceContext*);
	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;
	int _vertexCount;
	int _indexCount;
};