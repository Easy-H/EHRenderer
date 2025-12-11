#pragma once;

#include <directxmath.h>

using namespace DirectX;

class LightClass {

private:
	XMFLOAT4 _diffuseColor;
	XMFLOAT4 _ambientColor;
	XMFLOAT4 _specularColor;
	float _specularPower;
	XMFLOAT3 _direction;
	XMFLOAT4 _position;
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetPosition(float, float, float);

	void SetDiffuseColor(float, float, float, float);
	void SetAmbientColor(float, float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetPosition() { return _position; }

	XMFLOAT4 GetDiffuseColor() { return _diffuseColor; }
	XMFLOAT4 GetAmbientColor() { return _ambientColor; }
	XMFLOAT4 GetSpecularColor() { return _specularColor; }
	float GetSpecularPower() { return _specularPower; }
	XMFLOAT3 GetDirection() { return _direction; }


};