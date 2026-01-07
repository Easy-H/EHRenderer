#pragma once

#include <dinput.h>
#include <wrl.h>

using namespace Microsoft::WRL;

#define DIRECTINPUT_VERSION 0x0800

class InputClass {
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	bool IsLeftArrowPressed();
	bool IsRightArrowPressed();

	void GetMouseLocation(int&, int&);
	bool IsMousePressed();
private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

	ComPtr<IDirectInput8> _directInput;
	ComPtr<IDirectInputDevice8> _keyboard;
	ComPtr<IDirectInputDevice8> _mouse;

	unsigned char _keyboardState[256];
	DIMOUSESTATE _mouseState;

	int _screenWidth;
	int _screenHeight;
	int _mouseX;
	int _mouseY;
	
};