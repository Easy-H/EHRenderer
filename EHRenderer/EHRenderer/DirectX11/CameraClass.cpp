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

	Render();
}

void CameraClass::SetLookAt(float x, float y, float z)
{
	XMFLOAT3 up{ 0.f, 1.f, 0.f };

	XMVECTOR upVector = XMLoadFloat3(&up);
	XMFLOAT3 position{ _positionX, _positionY, _positionZ };
	XMVECTOR positionVector = XMLoadFloat3(&position);

	XMFLOAT3 lookAt = XMFLOAT3(x, y, z);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

}

void CameraClass::SetProjectionParameters(
	float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	_projectionMatrix = XMMatrixPerspectiveFovLH(
		fieldOfView, aspectRatio, nearPlane, farPlane);
}

void CameraClass::SetRotation(float x, float y, float z)
{
	_rotationX = x;
	_rotationY = y;
	_rotationZ = z;

	Render();
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
	XMFLOAT3 up{0.f, 1.f, 0.f };

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

void CameraClass::GetProjectionMatrix(XMMATRIX& projectionMatrix) {
	projectionMatrix = _projectionMatrix;
}

void CameraClass::RenderRefleciton(float height)
{
	XMFLOAT3 up, position, lookAt;

	up.x = 0.f;
	up.y = 1.f;
	up.z = 0.f;

	XMVECTOR upVector = XMLoadFloat3(&up);

	position.x = _positionX;
	position.y = -_positionY + (height * 2.f);
	position.z = _positionZ;
	
	XMVECTOR positionVector = XMLoadFloat3(&position);

	lookAt.x = 0.f;
	lookAt.y = 0.f;
	lookAt.z = 1.f;

	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	float pitch = (-1.f * _rotationX) * 0.0174532925f;
	float yaw = _rotationY * 0.0174532925f;
	float roll = _rotationZ * 0.0174532925f;

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	_reflectionViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

}

void CameraClass::GetReflectionViewMatrix(XMMATRIX& reflectionViewMatrix)
{
	reflectionViewMatrix = _reflectionViewMatrix;
}
