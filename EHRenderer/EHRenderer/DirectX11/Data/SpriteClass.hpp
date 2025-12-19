#pragma once;

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include <memory>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

class TextureClass;

class SpriteClass {
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};
public:
	SpriteClass();
	SpriteClass(const SpriteClass&);
	~SpriteClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, char*, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	void SetRenderLocation(int, int);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*);
	bool RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;

	int _vertexCount;
	int _indexCount;
	int _screenWidth;
	int _screenHeight;
	int _spriteWidth;
	int _spriteHeight;
	int _renderX;
	int _renderY;
	int _prevPosX;
	int _prevPosY;

	unique_ptr<TextureClass> _texture;

};