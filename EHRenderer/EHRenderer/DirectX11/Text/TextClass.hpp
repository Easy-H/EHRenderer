#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class FontClass;

class TextClass {
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, int, FontClass*, char*, int, int, float, float, float);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	
	int GetIndexCount();

	bool UpdateText(ID3D11DeviceContext*, FontClass*, char*, int, int, float, float, float);
	XMFLOAT4 GetPixelColor();

private:
	bool InitializeBuffers(ID3D11Device*, ID3D11DeviceContext*, FontClass*, char*, int, int, float, float, float);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;
	int _screenWidth, _screenHeight, _maxLength, _vertexCount, _indexCount;
	XMFLOAT4 _pixelColor;

};