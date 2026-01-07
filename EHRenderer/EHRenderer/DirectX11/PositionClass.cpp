#include "PositionClass.hpp"

PositionClass::PositionClass()
{
	_frameTime = 0.f;
	_rotationY = 0.f;
	_leftTurnSpeed = 0.f;
	_rightTurnSpeed = 0.f;
}

PositionClass::PositionClass(const PositionClass&)
{
}

PositionClass::~PositionClass()
{
}

void PositionClass::SetFrameTime(float time)
{
	_frameTime = time;
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
