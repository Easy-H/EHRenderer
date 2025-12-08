#include "TextureClass.hpp"

TextureClass::TextureClass()
{
}

TextureClass::TextureClass(const TextureClass&)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device,
	ID3D11DeviceContext* deviceContext, char* filename)
{
	std::unique_ptr<char[]> targaData = NewLoadTarga32Bit(filename);

	if (targaData == nullptr) return false;

	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Height = _height;
	textureDesc.Width = _width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, _texture.GetAddressOf())))
	{
		return false;
	}

	unsigned int rowPitch = (_width * 4) * sizeof(unsigned char);

	deviceContext->UpdateSubresource(_texture.Get(), 0, nullptr, targaData.get(), rowPitch, 0);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	if (FAILED(device->CreateShaderResourceView(_texture.Get(), &srvDesc, _textureView.GetAddressOf()))) {
		return false;
	}

	deviceContext->GenerateMips(_textureView.Get());

	return true;

}

void TextureClass::Shutdown()
{

}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return _textureView.Get();
}
std::unique_ptr<char[]> TextureClass::NewLoadTarga32Bit(char* filename)
{
	FILE* filePtr;

	if (fopen_s(&filePtr, filename, "rb")) {
		return nullptr;
	}

	TargaHeader targaFileHeader;
	unsigned int count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);

	if (count != 1) return nullptr;

	_height = (int)targaFileHeader.height;
	_width = (int)targaFileHeader.width;
	int bpp = (int)targaFileHeader.bpp;

	if (bpp != 32) return nullptr;

	int imageSize = _width * _height * 4;

	std::unique_ptr<unsigned char[]> targaImage = std::make_unique<unsigned char[]>(imageSize);
	count = (unsigned int)fread(targaImage.get(), 1, imageSize, filePtr);

	if (count != imageSize) return nullptr;

	if (fclose(filePtr) != 0) return nullptr;

	std::unique_ptr<char[]> retval = std::make_unique<char[]>(imageSize);
	int index = 0;
	int k = (_width * _height * 4) - (_width * 4);

	for (int j = 0; j < _height; j++) {
		for (int i = 0; i < _width; i++) {
			retval[index + 0] = targaImage[k + 2];
			retval[index + 1] = targaImage[k + 1];
			retval[index + 2] = targaImage[k + 0];
			retval[index + 3] = targaImage[k + 3];

			k += 4;
			index += 4;
		}
		k -= _width * 8;
	}

	return move(retval);
}

int TextureClass::GetWidth()
{
	return _width;
}

int TextureClass::GetHeight()
{
	return _height;
}
