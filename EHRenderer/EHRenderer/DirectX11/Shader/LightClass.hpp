#pragma once;

#include <directxmath.h>

using namespace DirectX;

class LightClass {

private:
	XMFLOAT4 _diffuseColor;
	XMFLOAT3 _direction;
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetDiffuseColor() { return _diffuseColor; }
	XMFLOAT3 GetDirection() { return _direction; }


};