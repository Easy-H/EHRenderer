#include "ApplicationClass.hpp"

#include "DirectX11/D3DClass.hpp"
#include "DirectX11/CameraClass.hpp"
#include "DirectX11/Model/ModelClass.hpp"
#include "DirectX11/Model/SimpleModelClass.hpp"
#include "DirectX11/Shader/Color/ColorShaderClass.hpp"
#include "DirectX11/Shader/Texture/TextureShaderClass.hpp"
#include "DirectX11/Shader/Light/LightShaderClass.hpp"
#include "DirectX11/Shader/LightClass.hpp"

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

	char modelFilename[128];
	strcpy_s(modelFilename, "./Assets/Cube.txt");

	char textureFilename[128];
	strcpy_s(textureFilename, "./Assets/stone01.tga");

	if (!_model->Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), modelFilename, textureFilename)) {
		MessageBox(hwnd, "Could not Initilalize the model object", "Error", MB_OK);
		return false;
	}

	_lightShader = std::make_unique<LightShaderClass>();
	if (!_lightShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_light = std::make_unique<LightClass>();
	_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	_light->SetDirection(0.f, 0.f, 1.f);
	/**/

	/*
	_textureShader = std::make_unique<TextureShaderClass>();
	if (!_textureShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	/*
	_colorShader = std::make_unique<ColorShaderClass>();
	if (!_colorShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Could not Initilalize the color shader object", "Error", MB_OK);
		return false;
	}*/

	return true;
}

void ApplicationClass::Shutdown() {
	_direct3D->Shutdown();
}

bool ApplicationClass::Frame()
{
	static float rotation = 0.f;

	//rotation -= 0.0174532925f * 0.1f;
	rotation -= 0.0174532925f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	return Render(rotation);
}

bool ApplicationClass::Render(float rotation)
{
	_direct3D->BeginScene(0.f, 0.f, 0.f, 1.0f);

	_camera->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	_direct3D->GetWorldMatrix(worldMatrix);
	_camera->GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixRotationY(rotation);

	_model->Render(_direct3D->GetDeviceContext());

	/**/
	if (!_lightShader->Render(
		_direct3D->GetDeviceContext(), _model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _model->GetTexture(),
		_light->GetDirection(), _light->GetDiffuseColor())) {
		return false;
	}

	/*
	if (!_textureShader->Render(
		_direct3D->GetDeviceContext(), _model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _model->GetTexture())) {
		return false;
	}*/

	/*
	if (!_colorShader->Render(_direct3D->GetDeviceContext(), _model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix)) {
		return false;
	}*/

	_direct3D->EndScene();
	return true;
}