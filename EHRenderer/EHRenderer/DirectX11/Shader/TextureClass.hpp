#pragma once

#include <d3d11.h>
#include <stdio.h>
#include <wrl.h>
#include <memory>

using namespace Microsoft::WRL;

class TextureClass {
private:
	struct TargaHeader {
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();
	int GetWidth();
	int GetHeight();

private:
	std::unique_ptr<char[]> NewLoadTarga32Bit(char*);
	bool LoadTarga32Bit(char*);

	ComPtr<ID3D11Texture2D> _texture;
	ComPtr<ID3D11ShaderResourceView> _textureView;
	int _width;
	int _height;

};