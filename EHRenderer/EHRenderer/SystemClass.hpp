#pragma once

#include <windows.h>
#include <memory>

class InputClass;
class ApplicationClass;

class SystemClass {
public:
	SystemClass();
	SystemClass(const SystemClass&);
	
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

	LPCSTR _applicationName;
	HINSTANCE _hInstance;
	HWND _hwnd;

	std::unique_ptr<InputClass> _input;
	std::unique_ptr<ApplicationClass> _application;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;