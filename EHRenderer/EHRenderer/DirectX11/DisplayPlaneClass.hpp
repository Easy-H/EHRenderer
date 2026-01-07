#pragma once

#include "D3DClass.hpp"

class DisplayPlaneClass {
private:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
public:
	DisplayPlaneClass();
	DisplayPlaneClass(const DisplayPlaneClass&);	
	~DisplayPlaneClass();

	bool Initialize(ID3D11Device*, float, float);
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*, float, float);
	void RenderBuffers(ID3D11DeviceContext*);

	ComPtr<ID3D11Buffer> _vertexBuffer, _indexBuffer;
	int _vertexCount, _indexCount;
};