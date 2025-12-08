#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include <memory>

using namespace DirectX;
using namespace Microsoft::WRL;

class TextureClass;

class SimpleModelClass {
private:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};
public:
	SimpleModelClass();
	SimpleModelClass(const SimpleModelClass&);
	~SimpleModelClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;
	int _vertexCount;
	int _indexCount;

	std::unique_ptr<TextureClass> _texture;

};