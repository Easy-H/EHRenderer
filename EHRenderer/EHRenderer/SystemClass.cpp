
#include "SystemClass.hpp"

#include "inputclass.hpp"
#include "applicationclass.hpp"

SystemClass::SystemClass()
{
}

SystemClass::SystemClass(const SystemClass&)
{
}

SystemClass::~SystemClass()
{

}

bool SystemClass::Initialize()
{
	int screenWidth = 0;
	int screenHeight = 0;

	InitializeWindows(screenWidth, screenHeight);

	_input = std::make_unique<InputClass>();
	_input->Initialize();

	_application = std::make_unique<ApplicationClass>();

	return _application->Initialize(screenWidth, screenHeight, _hwnd);
}


void SystemClass::Shutdown()
{

}

void SystemClass::Run()
{
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	while (true) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) break;
		if (!Frame()) break;
	}
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg) {
	case WM_KEYDOWN:
		_input->KeyDown((unsigned int)wparam);
		return 0;
	case WM_KEYUP:
		_input->KeyUp((unsigned int) wparam);
		return 0;
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

bool SystemClass::Frame()
{
	if (_input->IsKeyDown(VK_ESCAPE)) return false;
	return _application->Frame();
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc{};
	DEVMODE dmScreenSettings;
	int posX, posY;

	ApplicationHandle = this;
	_hInstance = GetModuleHandle(nullptr);
	_applicationName = "Engine";

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = _applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (FULL_SCREEN) {
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);

		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsWidth = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;

	}
	else {
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}

	_hwnd = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, nullptr, nullptr, _hInstance, nullptr);

	ShowWindow(_hwnd, SW_SHOW);

	SetForegroundWindow(_hwnd);
	SetFocus(_hwnd);

	ShowCursor(false);

}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN) {
		ChangeDisplaySettings(nullptr, 0);
	}

	DestroyWindow(_hwnd);
	_hwnd = nullptr;

	UnregisterClass(_applicationName, _hInstance);
	_hInstance = nullptr;

	ApplicationHandle = nullptr;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch (umessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
}