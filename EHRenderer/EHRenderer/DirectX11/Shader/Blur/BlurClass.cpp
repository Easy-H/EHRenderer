#include "BlurClass.hpp"

BlurClass::BlurClass()
{
}

BlurClass::BlurClass(const BlurClass&)
{
}

BlurClass::~BlurClass()
{
}

bool BlurClass::Initialize(D3DClass*, int, int, float, float, int, int)
{
	return false;
}

bool BlurClass::BlurTexture(D3DClass*, CameraClass*, RenderTextureClass*, TextureShaderClass*, BlurShaderClass*)
{
	return false;
}
