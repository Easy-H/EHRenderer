#include "ShaderManager.hpp"

#include "./Shader/Light/LightShaderClass.hpp"
#include "./Shader/NormalMap/NormalMapShaderClass.hpp"
#include "./Shader/Texture/TextureShaderClass.hpp"

ShaderManagerClass::ShaderManagerClass()
{
}

ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass&)
{
}

ShaderManagerClass::~ShaderManagerClass()
{
}

bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd) {
	_textureShader = make_unique<TextureShaderClass>();

	if (!_textureShader->Initialize()) {
		return false;
	}

	_lightShader = make_unique<LightShaderClass>();

	if (!_lightShader->Initialize(device, hwnd)) {
		return false;
	}

	_normalMapShader = make_unique<NormalMapShaderClass>();

	if (!_normalMapShader->Initialize(device, hwnd)) {
		return false;
	}

	return true;

}

bool ShaderManagerClass::RenderTextureShader(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix , XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture)
{
	return _textureShader->Render(indexCount);
}

bool ShaderManagerClass::RenderLightShader(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	return false;
	/*
	return _lightShader->Render(deviceContext, indexCount,
		worldMatrix, viewMatrix, projectionMatrix,
		texture, lightDirection, diffuseColor);*/
}

bool ShaderManagerClass::RenderNormalMapShader(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normalTexture,
	XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	return _normalMapShader->Render(deviceContext, indexCount,
		worldMatrix, viewMatrix, projectionMatrix,
		texture, normalTexture, lightDirection, diffuseColor);
}
