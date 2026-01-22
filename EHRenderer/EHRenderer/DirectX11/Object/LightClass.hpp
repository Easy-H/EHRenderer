#pragma once;

#include <directxmath.h>
#include "CameraClass.hpp"

using namespace DirectX;

class LightClass : public CameraClass {

private:
	XMFLOAT4 _diffuseColor;
	XMFLOAT4 _ambientColor;
	XMFLOAT4 _specularColor;
	float _specularPower;
	XMFLOAT3 _direction;

public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetAmbientColor(float, float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetDiffuseColor() { return _diffuseColor; }
	XMFLOAT4 GetAmbientColor() { return _ambientColor; }
	XMFLOAT4 GetSpecularColor() { return _specularColor; }
	float GetSpecularPower() { return _specularPower; }
	XMFLOAT3 GetDirection() { return _direction; }


};