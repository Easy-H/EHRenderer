#include "ApplicationClass.hpp"

#include "DirectX11/InputClass.hpp"
#include "DirectX11/D3DClass.hpp"
#include "DirectX11/CameraClass.hpp"
#include "DirectX11/Data/ModelClass.hpp"
#include "DirectX11/Data/TextureClass.hpp"
#include "DirectX11/Data/SimpleModelClass.hpp"
#include "DirectX11/Data/SpriteClass.hpp"
#include "DirectX11/Data/FontClass.hpp"
#include "DirectX11/Data/TextClass.hpp"

#include "DirectX11/Shader/Color/ColorShaderClass.hpp"
#include "DirectX11/Shader/Texture/TextureShaderClass.hpp"
#include "DirectX11/Shader/MultiTexture/MultiTextureShaderClass.hpp"
#include "DirectX11/Shader/LightMap/LightMapShaderClass.hpp"
#include "DirectX11/Shader/Light/LightShaderClass.hpp"
#include "DirectX11/Shader/LightClass.hpp"
#include "DirectX11/Shader/Font/FontShaderClass.hpp"

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
	strcpy_s(modelFilename, "./Assets/cube.txt");

	char textureFilename1[128];
	strcpy_s(textureFilename1, "./Assets/stone01.tga");
	char textureFilename2[128];
	strcpy_s(textureFilename2, "./Assets/dirt01.tga");
	char textureFilename3[128];
	strcpy_s(textureFilename3, "./Assets/light01.tga");

	if (!_model->Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		modelFilename, textureFilename1)) {
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

	_lightMapShader = std::make_unique<LightMapShaderClass>();

	if (!_lightMapShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}


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

	_multiTextureShader = std::make_unique<MultiTextureShaderClass>();
	
	if (!_multiTextureShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}


	_textures = make_unique<TextureClass[]>(3);

	if (!_textures[0].Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		textureFilename1))
		return false;

	if (!_textures[1].Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		textureFilename2))
		return false;

	if (!_textures[2].Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		textureFilename3))
		return false;

	_stringCount = 3;

	char testString1[32];
	char testString2[32];
	char testString3[32];

	strcpy_s(testString1, "Mouse X: 0");
	strcpy_s(testString2, "Mouse Y: 0");
	strcpy_s(testString3, "Mouse Button: No");

	_textStrings = make_unique<TextClass[]>(_stringCount);

	if (!_textStrings[0].Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		screenWidth, screenHeight, 32, _font.get(), testString1, 10, 10, 0.f, 1.f, 0.f)) {
		MessageBox(hwnd, "Could not Initilalize the Text1", "Error", MB_OK);
		return false;
	}

	if (!_textStrings[1].Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		screenWidth, screenHeight, 32, _font.get(), testString2, 10, 50, 1.f, 1.f, 0.f)) {
		MessageBox(hwnd, "Could not Initilalize the Text1", "Error", MB_OK);
		return false;
	}

	if (!_textStrings[2].Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		screenWidth, screenHeight, 32, _font.get(), testString3, 10, 90, 0.f, 1.f, 1.f)) {
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

bool ApplicationClass::Frame(InputClass& input)
{
	if (input.IsEscapePressed()) return false;

	int mouseX, mouseY;
	input.GetMouseLocation(mouseX, mouseY);
	bool mouseDown = input.IsMousePressed();

	if (!UpdateMouseStrings(mouseX, mouseY, mouseDown)) return false;

	return Render();
}

bool ApplicationClass::Render()
{
	XMMATRIX rotateMatrix, translateMatrix, scaleMatrix, srMatrix;

	_direct3D->BeginScene(0.f, 0.f, 0.f, 1.0f);

	_camera->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	_direct3D->GetWorldMatrix(worldMatrix);
	_camera->GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);
	_direct3D->GetOrthoMatrix(orthoMatrix);

	
	_model->Render(_direct3D->GetDeviceContext());

	if (!_lightMapShader->Render(_direct3D->GetDeviceContext(), _model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix,
		_textures[0].GetTexture(), _textures[2].GetTexture())) {
		return false;
	}

	/*
	if(!_multiTextureShader->Render(_direct3D->GetDeviceContext(), _model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix,
		_textures[0].GetTexture(), _textures[1].GetTexture()))
		return false;
*/
	/*
	XMFLOAT4 diffuseColor[4]{}, lightPosition[4]{};

	for (int i = 0; i < _numLights; i++) {
		diffuseColor[i] = _lights[i].GetDiffuseColor();
		lightPosition[i] = _lights[i].GetPosition();
	}

	if (!_lightShader->Render(
		_direct3D->GetDeviceContext(), _model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _texture2->GetTexture(),
		diffuseColor, lightPosition)) {
		return false;
	}/**/

	_direct3D->TurnZBufferOff();

	if (!_sprite->Render(_direct3D->GetDeviceContext())) return false;

	if (!_textureShader->Render(_direct3D->GetDeviceContext(), _sprite->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, _textures[0].GetTexture())) return false;

	_direct3D->EnableAlphaBlending();

	for (int i = 0; i < _stringCount; i++) {
		_textStrings[i].Render(_direct3D->GetDeviceContext());
		if (!_fontShader->Render(_direct3D->GetDeviceContext(), _textStrings[i].GetIndexCount(),
			worldMatrix, viewMatrix, orthoMatrix, _font->GetTexture(), _textStrings[i].GetPixelColor()))
			return false;
	}

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

bool ApplicationClass::UpdateMouseStrings(int mouseX, int mouseY, bool mouseDown)
{
	char tempString[16], finalString[32];

	sprintf_s(tempString, "%d", mouseX);
	strcpy_s(finalString, "MouseX: ");
	strcat_s(finalString, tempString);

	if (!_textStrings[0].UpdateText(_direct3D->GetDeviceContext(), _font.get(),
		finalString, 10, 10, 1.0f, 1.0f, 1.0f))
		return false;

	sprintf_s(tempString, "%d", mouseY);
	strcpy_s(finalString, "MouseY: ");
	strcat_s(finalString, tempString);

	if (!_textStrings[1].UpdateText(_direct3D->GetDeviceContext(), _font.get(),
		finalString, 10, 50, 1.0f, 1.0f, 0.0f))
		return false;

	if (mouseDown) {
		strcpy_s(finalString, "MouseButton: Yes");
	}
	else {
		strcpy_s(finalString, "MouseButton: No");
	}

	if (!_textStrings[2].UpdateText(_direct3D->GetDeviceContext(), _font.get(),
		finalString, 10, 90, 0.0f, 1.0f, 1.0f))
		return false;

	return true;
}
