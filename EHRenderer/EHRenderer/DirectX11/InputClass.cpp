#include "InputClass.hpp"


InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass&)
{
}

InputClass::~InputClass()
{
	if (_keyboard != nullptr) {
		_keyboard->Unacquire();
	}
	if (_mouse != nullptr) {
		_mouse->Unacquire();
	}
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth , int screenHeight)
{

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_mouseX = 0;
	_mouseY = 0;

	if (FAILED(DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)_directInput.GetAddressOf(), nullptr)))
		return false;

	if (FAILED(_directInput->CreateDevice(GUID_SysKeyboard, _keyboard.GetAddressOf(), nullptr)))
		return false;

	if (FAILED(_keyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if (FAILED(_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		return false;

	if (FAILED(_keyboard->Acquire()))
		return false;



	if (FAILED(_directInput->CreateDevice(GUID_SysMouse, _mouse.GetAddressOf(), nullptr)))
		return false;

	if (FAILED(_mouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	if (FAILED(_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	if (FAILED(_mouse->Acquire()))
		return false;

	return true;

}

void InputClass::Shutdown() {
}

bool InputClass::Frame() {

	if (!ReadKeyboard()) return false;
	if (!ReadMouse()) return false;

	ProcessInput();

	return true;

}

bool InputClass::ReadKeyboard()
{
	HRESULT result = _keyboard->GetDeviceState(sizeof(_keyboardState), (LPVOID)&_keyboardState);
	if (FAILED(result)) {
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			_keyboard->Acquire();
		}
		else return false;
	}
	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result = _mouse->GetDeviceState(sizeof(_DIMOUSESTATE), (LPVOID)&_mouseState);
	if (FAILED(result)) {
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			_mouse->Acquire();
		}
		else return false;
	}
	return true;
}

void InputClass::ProcessInput()
{
	_mouseX += _mouseState.lX;
	_mouseY += _mouseState.lY;

	if (_mouseX < 0) _mouseX = 0;
	if (_mouseY < 0) _mouseY = 0;

	if (_mouseX > _screenWidth) _mouseX = _screenWidth;
	if (_mouseY > _screenHeight) _mouseY = _screenHeight;
}

bool InputClass::IsEscapePressed()
{
	if (_keyboardState[DIK_ESCAPE] & 0x80) return true;
	return false;
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = _mouseX;
	mouseY = _mouseY;
}

bool InputClass::IsMousePressed()
{
	if (_mouseState.rgbButtons[0] & 0x80) return true;
	return false;
}
