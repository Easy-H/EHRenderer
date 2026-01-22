#pragma once;

#include <directxmath.h>

using namespace DirectX;

class CameraClass {
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetLookAt(float, float, float);
	void SetProjectionParameters(float, float, float, float);
	void SetOrthoParameters(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();

	void GetViewMatrix(XMMATRIX&);
	void GetProjectionMatrix(XMMATRIX&);

	void RenderRefleciton(float);
	void GetReflectionViewMatrix(XMMATRIX&);

private:
	float _positionX, _positionY, _positionZ;
	float _rotationX, _rotationY, _rotationZ;

	bool _isOrtho;

	XMMATRIX _viewMatrix, _projectionMatrix;
	XMMATRIX _orthMatrix;
	XMMATRIX _reflectionViewMatrix;
};