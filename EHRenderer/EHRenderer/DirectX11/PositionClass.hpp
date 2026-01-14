#pragma once

class PositionClass {
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();
	
	void SetFrameTime(float);

	void SetPosition(float, float, float);
	void GetPosition(float&, float&, float&);

	void GetRotation(float&);

	void TurnLeft(bool);
	void TurnRight(bool);

	void MoveLeft(bool);
	void MoveRight(bool);

private:
	float _frameTime;
	float _rotationX, _rotationY, _rotationZ;
	float _positionX, _positionY, _positionZ;

	float _leftTurnSpeed;
	float _rightTurnSpeed;
	float _leftSpeed;
	float _rightSpeed;
};