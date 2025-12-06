#include "ApplicationClass.hpp"

#include "DirectX11/D3DClass.hpp"
#include "DirectX11/CameraClass.hpp"
#include "DirectX11/Model/ModelClass.hpp"
#include "DirectX11/Shader/Color/ColorShaderClass.hpp"

ApplicationClass::ApplicationClass() {

}
ApplicationClass::ApplicationClass(const ApplicationClass&)
{
}

ApplicationClass::~ApplicationClass()
{

}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	_direct3D = std::make_unique<D3DClass>();
	_direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	
	_camera = std::make_unique<CameraClass>();
	_camera->SetPosition(0.f, 0.f, -5.f);

	_model = std::make_unique<ModelClass>();
	if (!_model->Initialize(_direct3D->GetDevice())) {
		MessageBox(hwnd, "Could not Initilalize the model object", "Error", MB_OK);
		return false;
	}

	_colorShader = std::make_unique<ColorShaderClass>();
	if (!_colorShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Could not Initilalize the color shader object", "Error", MB_OK);
		return false;
	}

	return true;
}

void ApplicationClass::Shutdown() {
	_direct3D->Shutdown();
}

bool ApplicationClass::Frame()
{
	return Render();
}

bool ApplicationClass::Render()
{
	_direct3D->BeginScene(0.f, 0.f, 0.f, 1.0f);

	_camera->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	_direct3D->GetWorldMatrix(worldMatrix);
	_camera->GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);

	_model->Render(_direct3D->GetDeviceContext());

	if (!_colorShader->Render(_direct3D->GetDeviceContext(), _model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix)) {
		return false;
	}

	_direct3D->EndScene();
	return true;
}