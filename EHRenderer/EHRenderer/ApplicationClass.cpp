#include "ApplicationClass.hpp"

#include "DirectX11/D3DClass.hpp"
#include "DirectX11/CameraClass.hpp"
#include "DirectX11/Model/ModelClass.hpp"
#include "DirectX11/Model/SimpleModelClass.hpp"
#include "DirectX11/Sprite/SpriteClass.hpp"
#include "DirectX11/Shader/Color/ColorShaderClass.hpp"
#include "DirectX11/Shader/Texture/TextureShaderClass.hpp"
#include "DirectX11/Shader/Light/LightShaderClass.hpp"
#include "DirectX11/Shader/LightClass.hpp"
#include "DirectX11/Shader/Font/FontShaderClass.hpp"
#include "DirectX11/Font/FontClass.hpp"

#include "DirectX11/Text/TextClass.hpp"

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
	_camera->SetPosition(0.f, 2.f, -12.f);

	_model = std::make_unique<ModelClass>();

	char modelFilename[128];
	strcpy_s(modelFilename, "./Assets/plane.txt");

	char textureFilename[128];
	strcpy_s(textureFilename, "./Assets/stone01.tga");

	if (!_model->Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		modelFilename, textureFilename)) {
		MessageBox(hwnd, "Could not Initilalize the model object", "Error", MB_OK);
		return false;
	}

	_sprite = make_unique<SpriteClass>();

	char spriteFilename[128];

	strcpy_s(spriteFilename, "./Assets/stone01.tga");

	if (!_sprite->Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		screenWidth, screenHeight, spriteFilename, 500, 50))
		return false;

	_lightShader = std::make_unique<LightShaderClass>();
	if (!_lightShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Could not Initilalize the shader", "Error", MB_OK);
		return false;
	}


	_numLights = 4;

	_lights = make_unique<LightClass[]>(_numLights);

	_lights[0].SetDiffuseColor(1.0f, 0.f, 0.f, 1.f);
	_lights[0].SetPosition(-3.f, 1.f, 3.f);

	_lights[1].SetDiffuseColor(0.0f, 1.f, 0.f, 1.f);
	_lights[1].SetPosition(3.f, 1.f, 3.f);

	_lights[2].SetDiffuseColor(0.0f, 0.f, 1.f, 1.f);
	_lights[2].SetPosition(-3.f, 1.f, -3.f);

	_lights[3].SetDiffuseColor(1.0f, 1.f, 1.f, 1.f);
	_lights[3].SetPosition(3.f, 1.f, -3.f);

	_textureShader = std::make_unique<TextureShaderClass>();
	if (!_textureShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}
	/**/


	_fontShader = make_unique<FontShaderClass>();
	if (!_fontShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Error", "Error", MB_OK);
		return false;
	}

	_font = make_unique<FontClass>();
	if (!_font->Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), 0)) {
		MessageBox(hwnd, "Error", "Error", MB_OK);
		return false;
	}

	char testString1[32];

	strcpy_s(testString1, "Hello");
	_textString1 = make_unique<TextClass>();

	if (!_textString1->Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		screenWidth, screenHeight, 32, _font.get(), testString1, 10, 10, 0.f, 1.f, 0.f)) {
		MessageBox(hwnd, "Could not Initilalize the Text1", "Error", MB_OK);
		return false;
	}

	char testString2[32];

	strcpy_s(testString2, "GoodBye");
	_textString2 = make_unique<TextClass>();

	if (!_textString2->Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		screenWidth, screenHeight, 32, _font.get(), testString2, 10, 50, 1.f, 1.f, 0.f)) {
		MessageBox(hwnd, "Could not Initilalize the Text1", "Error", MB_OK);
		return false;
	}

	/*
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
	XMMATRIX rotateMatrix, translateMatrix, scaleMatrix, srMatrix;

	_direct3D->BeginScene(0.f, 0.f, 0.f, 1.0f);

	_camera->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	_direct3D->GetWorldMatrix(worldMatrix);
	_camera->GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);
	_direct3D->GetOrthoMatrix(orthoMatrix);

	/*
	rotateMatrix = XMMatrixRotationY(rotation);
	translateMatrix = XMMatrixTranslation(-2.0f, 0.f, 0.f);

	worldMatrix = XMMatrixMultiply(rotateMatrix, translateMatrix);
	*/

	_model->Render(_direct3D->GetDeviceContext());

	XMFLOAT4 diffuseColor[4]{}, lightPosition[4]{};

	for (int i = 0; i < _numLights; i++) {
		diffuseColor[i] = _lights[i].GetDiffuseColor();
		lightPosition[i] = _lights[i].GetPosition();
	}


	if (!_lightShader->Render(
		_direct3D->GetDeviceContext(), _model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _model->GetTexture(),
		diffuseColor, lightPosition)) {
		return false;
	}



	_direct3D->TurnZBufferOff();

	if (!_sprite->Render(_direct3D->GetDeviceContext())) return false;

	if (!_textureShader->Render(_direct3D->GetDeviceContext(), _sprite->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, _sprite->GetTexture())) return false;

	_direct3D->EnableAlphaBlending();

	_textString1->Render(_direct3D->GetDeviceContext());
	if (!_fontShader->Render(_direct3D->GetDeviceContext(), _textString1->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, _font->GetTexture(), _textString1->GetPixelColor()))
		return false;

	_textString2->Render(_direct3D->GetDeviceContext());

	if (!_fontShader->Render(_direct3D->GetDeviceContext(), _textString2->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, _font->GetTexture(), _textString2->GetPixelColor()))
		return false;

	_direct3D->TurnZBufferOn();
	_direct3D->DisableAlphaBlending();
	_direct3D->EndScene();

	return true;



	/*
	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	rotateMatrix = XMMatrixRotationY(rotation);
	translateMatrix = XMMatrixTranslation(2.f, 0.f, 0.f);

	srMatrix = XMMatrixMultiply(scaleMatrix, rotateMatrix);
	worldMatrix = XMMatrixMultiply(srMatrix, translateMatrix);

	_model->Render(_direct3D->GetDeviceContext());

	if (!_lightShader->Render(
		_direct3D->GetDeviceContext(), _model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _model->GetTexture(),
		_light->GetDirection(), _light->GetDiffuseColor(), _light->GetAmbientColor(),
		_camera->GetPosition(), _light->GetSpecularColor(), _light->GetSpecularPower())) {
		return false;
	}*/

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
}