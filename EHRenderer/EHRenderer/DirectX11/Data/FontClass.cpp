#include "FontClass.hpp"
#include "TextureClass.hpp"

FontClass::FontClass()
{
}

FontClass::FontClass(const FontClass&)
{
}

FontClass::~FontClass()
{
}


bool FontClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int fontChoice)
{
	char fontFilename[128];
	char fontTextureFilename[128];

	switch (fontChoice) {
	case 1:
	default:
		strcpy_s(fontFilename, "./Assets/font01.txt");
		strcpy_s(fontTextureFilename, "./Assets/font01.tga");
		_fontHeight = 32.f;
		_spaceSize = 3;
		break;
	}

	if (!LoadFontData(fontFilename)) {
		return false;
	}

	return LoadTexture(device, deviceContext, fontTextureFilename);
}

void FontClass::Shutdown() {
	
}

ID3D11ShaderResourceView* FontClass::GetTexture() {
	return _texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr = (VertexType*)vertices;
	int numLetters = (int)strlen(sentence);

	int index = 0;

	for (int i = 0; i < numLetters; i++) {
		int letter = ((int)sentence[i]) - 32;

		if (letter == 0) {
			drawX = drawX + _spaceSize;
			continue;
		}

		vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.f);
		vertexPtr[index].texture = XMFLOAT2(_font[letter].left, 0.f);
		index++;

		vertexPtr[index].position = XMFLOAT3(drawX + _font[letter].size, drawY - _fontHeight, 0.f);
		vertexPtr[index].texture = XMFLOAT2(_font[letter].right, 1.f);
		index++;

		vertexPtr[index].position = XMFLOAT3(drawX, drawY - _fontHeight, 0.f);
		vertexPtr[index].texture = XMFLOAT2(_font[letter].left, 1.f);
		index++;

		vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.f);
		vertexPtr[index].texture = XMFLOAT2(_font[letter].left, 0.f);
		index++;

		vertexPtr[index].position = XMFLOAT3(drawX + _font[letter].size, drawY, 0.f);
		vertexPtr[index].texture = XMFLOAT2(_font[letter].right, 0.f);
		index++;

		vertexPtr[index].position = XMFLOAT3(drawX + _font[letter].size, drawY - _fontHeight, 0.f);
		vertexPtr[index].texture = XMFLOAT2(_font[letter].right, 1.f);
		index++;

		drawX = drawX + _font[letter].size + 1.f;

	}
}

int FontClass::GetSentencePixelLength(char* sentence)
{
	int pixelLength = 0;
	int numLetters = (int)strlen(sentence);

	for (int i = 0; i < numLetters; i++) {
		int letter = ((int)sentence[i]) - 32;

		if (letter == 0) pixelLength += _spaceSize;
		else {
			pixelLength += _font[letter].size + 1;
		}
	}

	return 0;
}

int FontClass::GetFontHeight()
{
	return _fontHeight;
}

bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;

	_font = make_unique<FontType[]>(95);

	fin.open(filename);

	if (fin.fail()) return false;

	char temp;
	for (int i = 0; i < 95; i++) {
		fin.get(temp);
		while (temp != ' ') fin.get(temp);
		fin.get(temp);
		while (temp != ' ') fin.get(temp);
		fin >> _font[i].left;
		fin >> _font[i].right;
		fin >> _font[i].size;
	}

	fin.close();
	return true;
}

void FontClass::ReleaseFontData()
{
}


bool FontClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename) {
	
	_texture = make_unique<TextureClass>();
	return _texture->Initialize(device, deviceContext, filename);

}

void FontClass::ReleaseTexture() {
	
}