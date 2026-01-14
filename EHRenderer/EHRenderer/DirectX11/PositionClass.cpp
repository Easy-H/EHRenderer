#include "PositionClass.hpp"

#include <math.h>

PositionClass::PositionClass()
{
	_rotationX = 0.f;
	_rotationY = 0.f;
	_rotationZ = 0.f;

	_positionX = 0.f;
	_positionY = 0.f;
	_positionZ = 0.f;
	
	_frameTime = 0.f;

	_leftTurnSpeed = 0.f;
	_rightTurnSpeed = 0.f;

	_leftSpeed = 0.f;
	_rightSpeed = 0.f;
}

PositionClass::PositionClass(const PositionClass& target)
{
	_rotationX = target._rotationX;
	_rotationY = target._rotationY;
	_rotationZ = target._rotationZ;

	_positionX = target._positionX;
	_positionY = target._positionY;
	_positionZ = target._positionZ;
	
	_frameTime = target._frameTime;

	_leftTurnSpeed = target._leftTurnSpeed;
	_rightTurnSpeed = target._rightTurnSpeed;

	_leftSpeed = target._leftSpeed;
	_rightSpeed = target._rightSpeed;
}

PositionClass::~PositionClass()
{
}

void PositionClass::SetFrameTime(float time)
{
	_frameTime = time;
}

void PositionClass::SetPosition(float x, float y, float z)
{
	_positionX = x;
	_positionY = y;
	_positionZ = z;
}

void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = _positionX;
	y = _positionY;
	z = _positionZ;
}

void PositionClass::GetRotation(float& y)
{
	y = _rotationY;
}

void PositionClass::TurnLeft(bool keydown)
{
	if (keydown) {
		_leftTurnSpeed += _frameTime * 1.5f;
		if (_leftTurnSpeed > (_frameTime * 200.f)) {
			_leftTurnSpeed = _frameTime * 200.f;
		}
	}
	else {
		_leftTurnSpeed -= _frameTime * 1.f;
		if (_leftTurnSpeed < 0.f) {
			_leftTurnSpeed = 0.f;
		}
	}

	_rotationY -= _leftTurnSpeed;

	if (_rotationY < 0.f) {
		_rotationY += 360.f;
	}
}

void PositionClass::TurnRight(bool keydown)
{
	if (keydown) {
		_rightTurnSpeed += _frameTime * 1.5f;
		if (_rightTurnSpeed > (_frameTime * 200.f)) {
			_rightTurnSpeed = _frameTime * 200.f;
		}
	}
	else {
		_rightTurnSpeed -= _frameTime * 1.f;
		if (_rightTurnSpeed < 0.f) {
			_rightTurnSpeed = 0.f;
		}
	}

	_rotationY += _rightTurnSpeed;

	if (_rotationY > 360.f) {
		_rotationY -= 360.f;
	}
}

void PositionClass::MoveLeft(bool keydown)
{
	if (keydown) {
		_leftSpeed += _frameTime * 0.25f;
		if (_leftSpeed += _frameTime * 5.f) {
			_leftSpeed = _frameTime * 5.f;
		}
	}
	else {
		_leftSpeed -= _frameTime * 0.25f;
		if (_leftSpeed < 0.f) {
			_leftSpeed = 0.f;
		}
	}

	float radians = _rotationY * 0.174532925f;

	_positionX -= cosf(radians) * _leftSpeed;
	_positionZ -= sinf(radians) * _leftSpeed;
}

void PositionClass::MoveRight(bool keydown)
{
	if (keydown) {
		_rightSpeed += _frameTime * 0.25f;
		if (_rightSpeed += _frameTime * 5.f) {
			_rightSpeed = _frameTime * 5.f;
		}
	}
	else {
		_rightSpeed -= _frameTime * 0.25f;
		if (_rightSpeed < 0.f) {
			_rightSpeed = 0.f;
		}
	}

	float radians = _rotationY * 0.174532925f;

	_positionX += cosf(radians) * _rightSpeed;
	_positionZ += sinf(radians) * _rightSpeed;
}
