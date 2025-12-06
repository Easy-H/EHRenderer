#include "InputClass.hpp"

InputClass::InputClass()
{

}

InputClass::InputClass(const InputClass&)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize() {
	for (int i = 0; i < 256; i++) {
		_keys[i] = false;
	}
}

void InputClass::KeyDown(unsigned int input)
{
	_keys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
	_keys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int input)
{
	return _keys[input];
}
