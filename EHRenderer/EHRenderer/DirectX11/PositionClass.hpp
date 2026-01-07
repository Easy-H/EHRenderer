#pragma once

class PositionClass {
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();
	
	void SetFrameTime(float);
	void GetRotation(float&);

	void TurnLeft(bool);
	void TurnRight(bool);

private:
	float _frameTime;
	float _rotationY;
	float _leftTurnSpeed;
	float _rightTurnSpeed;
};