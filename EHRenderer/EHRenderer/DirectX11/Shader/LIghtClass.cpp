#include "LightClass.hpp"

LightClass::LightClass()
{
}

LightClass::LightClass(const LightClass&)
{
}

LightClass::~LightClass()
{
}

void LightClass::SetPosition(float x, float y, float z)
{
	_position = XMFLOAT3(x, y, z);
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	_diffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	_ambientColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	_specularColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetSpecularPower(float power)
{
	_specularPower = power;
}


void LightClass::SetDirection(float x, float y, float z)
{
	_direction = XMFLOAT3(x, y, z);
}