#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <memory>

using namespace DirectX;
using namespace std;

class TextureShaderClass;
class LightShaderClass;
class NormalMapShaderClass;

class ShaderManagerClass {
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device*, HWND);
	
	bool RenderTextureShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*);
	bool RenderLightShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
	bool RenderNormalMapShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);

private:
	unique_ptr<TextureShaderClass> _textureShader;
	unique_ptr<LightShaderClass> _lightShader;
	unique_ptr<NormalMapShaderClass> _normalMapShader;
};