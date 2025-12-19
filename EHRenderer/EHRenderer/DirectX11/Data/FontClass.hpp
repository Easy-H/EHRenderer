#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <memory>

using namespace DirectX;
using namespace std;

class TextureClass;

class FontClass {
private:
	struct FontType {
		float left;
		float right;
		int size;
	};

	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, char*, float, float);
	int GetSentencePixelLength(char*);
	int GetFontHeight();

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();

private:
	unique_ptr<FontType[]> _font;
	unique_ptr<TextureClass> _texture;
	float _fontHeight;
	int _spaceSize;

};