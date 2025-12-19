#include "SimpleInputClass.hpp"

SimpleInputClass::SimpleInputClass()
{

}

SimpleInputClass::SimpleInputClass(const SimpleInputClass&)
{
}

SimpleInputClass::~SimpleInputClass()
{
}

void SimpleInputClass::Initialize() {
	for (int i = 0; i < 256; i++) {
		_keys[i] = false;
	}
}

void SimpleInputClass::KeyDown(unsigned int input)
{
	_keys[input] = true;
}

void SimpleInputClass::KeyUp(unsigned int input)
{
	_keys[input] = false;
}

bool SimpleInputClass::IsKeyDown(unsigned int input)
{
	return _keys[input];
}
