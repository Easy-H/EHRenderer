#include "CameraClass.hpp"

CameraClass::CameraClass()
{
	_positionX = 0.f;
	_positionY = 0.f;
	_positionZ = 0.f;

	_rotationX = 0.f;
	_rotationY = 0.f;
	_rotationZ = 0.f;
}

CameraClass::CameraClass(const CameraClass&)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	_positionX = x;
	_positionY = y;
	_positionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	_rotationX = x;
	_rotationY = y;
	_rotationZ = z;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(_positionX, _positionY, _positionZ);
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(_rotationX, _rotationY, _rotationZ);
}

void CameraClass::Render()
{
	XMFLOAT3 up{
		0.f, 1.f, 0.f };

	XMVECTOR upVector = XMLoadFloat3(&up);
	
	XMFLOAT3 position{ _positionX, _positionY, _positionZ };
	XMVECTOR positionVector = XMLoadFloat3(&position);

	XMFLOAT3 lookAt{ 0.f, 0.f, 1.f };
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	float pitch = _rotationX * 0.0174532925f;
	float yaw = _rotationY * 0.0174532925f;
	float roll = _rotationZ * 0.0174532925f;
	
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = _viewMatrix;
}