#include "ApplicationClass.hpp"

#include "TimerClass.hpp"

#include "DirectX11/InputClass.hpp"
#include "DirectX11/D3DClass.hpp"
#include "DirectX11/CameraClass.hpp"
#include "DirectX11/Data/ModelClass.hpp"
#include "DirectX11/Data/TextureClass.hpp"
#include "DirectX11/Data/SimpleModelClass.hpp"
#include "DirectX11/Data/SpriteClass.hpp"
#include "DirectX11/Data/FontClass.hpp"
#include "DirectX11/Data/TextClass.hpp"

#include "DirectX11/ShaderManager.hpp"
#include "DirectX11/Shader/Color/ColorShaderClass.hpp"
#include "DirectX11/Shader/Texture/TextureShaderClass.hpp"
#include "DirectX11/Shader/MultiTexture/MultiTextureShaderClass.hpp"
#include "DirectX11/Shader/AlphaMap/AlphaMapShaderClass.hpp"
#include "DirectX11/Shader/NormalMap/NormalMapShaderClass.hpp"
#include "DirectX11/Shader/SpecMap/SpecMapShaderClass.hpp"
#include "DirectX11/Shader/LightMap/LightMapShaderClass.hpp"
#include "DirectX11/Shader/Light/LightShaderClass.hpp"
#include "DirectX11/Shader/LightClass.hpp"
#include "DirectX11/Shader/Font/FontShaderClass.hpp"
#include "DirectX11/Shader/Fog/FogShaderClass.hpp"
#include "DirectX11/Shader/ClipPlane/ClipPlaneShaderClass.hpp"
#include "DirectX11/Shader/Translate/TranslateShaderClass.hpp"
#include "DirectX11/Shader/Transparent/TransparentShaderClass.hpp"
#include "DirectX11/Shader/Reflection/ReflectionShaderClass.hpp"
#include "DirectX11/Shader/Water/WaterShaderClass.hpp"
#include "DirectX11/Shader/Refraction/RefractionShaderClass.hpp"

#include "DirectX11/FrustumClass.hpp"
#include "DirectX11/PositionClass.hpp"
#include "DirectX11/ModelListClass.hpp"

#include "DirectX11/RenderTextureClass.hpp"
#include "DirectX11/DisplayPlaneClass.hpp"

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
	_camera->SetPosition(-10.f, 6.f, -10.f);
	_camera->SetRotation(0.f, 45.f, 0.f);
	_camera->Render();
	_camera->GetViewMatrix(_baseViewMatrix);

	_models = std::make_unique<ModelClass[]>(4);

	char modelFilename[4][128]{};
	strcpy_s(modelFilename[0], "./Assets/ground.txt");
	strcpy_s(modelFilename[1], "./Assets/wall.txt");
	strcpy_s(modelFilename[2], "./Assets/bath.txt");
	strcpy_s(modelFilename[3], "./Assets/water.txt");

	for (int i = 0; i < 4; i++) {
		if (!_models[i].Initialize(_direct3D->GetDevice(),
			_direct3D->GetDeviceContext(), modelFilename[i])) {
			MessageBox(hwnd, "Could not Initilalize the model object", modelFilename[i], MB_OK);
			return false;
		}
	}

	_textures = make_unique<TextureClass[]>(5);
	
	char textureFilename[5][128]{};
	strcpy_s(textureFilename[0], "./Assets/ground01.tga");
	strcpy_s(textureFilename[1], "./Assets/wall01.tga");
	strcpy_s(textureFilename[2], "./Assets/marble01.tga");
	strcpy_s(textureFilename[3], "./Assets/water01.tga");
	strcpy_s(textureFilename[4], "./Assets/spec02.tga");

	for (int i = 0; i < 5; i++) {
		if (!_textures[i].Initialize(_direct3D->GetDevice(),
			_direct3D->GetDeviceContext(), textureFilename[i])) {
			MessageBox(hwnd, "Could not Initilalize the model object", textureFilename[i], MB_OK);
			return false;
		}
	}

	_sprite = make_unique<SpriteClass>();

	if (!_sprite->Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(),
		screenWidth, screenHeight, textureFilename[0], 500, 50)) {
		MessageBox(hwnd, "Could not Initilalize the model object", "Error", MB_OK);
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
	_lights[3].SetDirection(0.0f, 0.0f, 1.0f);

	_lights[3].SetPosition(3.f, 1.f, -3.f);
	_lights[3].SetSpecularColor(1.f, 1.f, 1.f, 1.f);
	_lights[3].SetSpecularPower(16.0f);

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

	_shaderManager = make_unique<ShaderManagerClass>();

	if (!_shaderManager->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_fogShader = make_unique<FogShaderClass>();

	if (!_fogShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}


	_clipPlaneShader = make_unique<ClipPlaneShaderClass>();

	if (!_clipPlaneShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_translateShader = make_unique<TranslateShaderClass>();

	if (!_translateShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}
	
	_transparentShader = make_unique<TransparentShaderClass>();

	if (!_transparentShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_reflectionShader = make_unique<ReflectionShaderClass>();

	if (!_reflectionShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_waterShader = make_unique<WaterShaderClass>();

	if (!_waterShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_refractionShader = make_unique<RefractionShaderClass>();

	if (!_refractionShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

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

	_modelList = make_unique<ModelListClass>();
	_modelList->Initialize(25);

	_timer = make_unique<TimerClass>();

	if (!_timer->Initialize()) return false;

	_position = make_unique<PositionClass>();
	_frustum = make_unique<FrustumClass>();

	/*
	/*
	_colorShader = std::make_unique<ColorShaderClass>();
	if (!_colorShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Could not Initilalize the color shader object", "Error", MB_OK);
		return false;
	}*/

	_renderTextures = make_unique<RenderTextureClass[]>(2);

	for (int i = 0; i < 2; i++) {
		if (!_renderTextures[i].Initialize(_direct3D->GetDevice(),
			screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1)) {
			return false;
		}
		
	}

	_displayPlane = make_unique<DisplayPlaneClass>();

	if (!_displayPlane->Initialize(_direct3D->GetDevice(), 1.f, 1.f)) {
		return false;
	}

	_waterHeight = 2.75f;
	_waterTranslation = 0.f;

	return true;
}

void ApplicationClass::Shutdown() {
	_direct3D->Shutdown();
}

bool ApplicationClass::Frame(InputClass& input)
{
	if (input.IsEscapePressed()) return false;

	_timer->Frame();
	_position->SetFrameTime(_timer->GetTime());
	_position->TurnLeft(input.IsLeftArrowPressed());
	_position->TurnRight(input.IsRightArrowPressed());

	_waterTranslation += 0.001f;
	if (_waterTranslation > 1.0f)
	{
		_waterTranslation -= 1.0f;
	}

	int mouseX, mouseY;
	input.GetMouseLocation(mouseX, mouseY);
	bool mouseDown = input.IsMousePressed();

	/*
	if (!UpdateMouseStrings(mouseX, mouseY, mouseDown)) return false;
	/**/

	if (!RenderRefractionToTexture()) {
		return false;
	}

	if (!RenderReflectionToTexture()) {
		return false;
	}

	return Render();
}

bool ApplicationClass::Render()
{

	float fogColor = 0.f;

	_direct3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

	_camera->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	_direct3D->GetWorldMatrix(worldMatrix);
	
	_camera->GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);
	_direct3D->GetOrthoMatrix(orthoMatrix);

	_direct3D->EnableAlphaBlending();
	_models[0].Render(_direct3D->GetDeviceContext());

	if (!_shaderManager->RenderTextureShader(_direct3D->GetDeviceContext(), _models[0].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[0].GetTexture())) {
		return false;
	}

	worldMatrix = XMMatrixTranslation(0.f, 6.f, 8.f);

	_models[1].Render(_direct3D->GetDeviceContext());

	if (!_shaderManager->RenderTextureShader(_direct3D->GetDeviceContext(), _models[1].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[1].GetTexture())) {
		return false;
	}
	
	_direct3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixTranslation(0.f, 2.f, 0.f);

	_models[2].Render(_direct3D->GetDeviceContext());

	if (!_shaderManager->RenderTextureShader(_direct3D->GetDeviceContext(), _models[2].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[2].GetTexture())) {
		return false;
	}


	XMMATRIX reflectionViewMatrix;
	_camera->GetReflectionViewMatrix(reflectionViewMatrix);

	_models[3].Render(_direct3D->GetDeviceContext());

	/*
	if (!_reflectionShader->Render(_direct3D->GetDeviceContext(), _models[1].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[1].GetTexture(),
		_renderTextures[1].GetShaderResourceView(), reflectionViewMatrix)) {
		return false;
	}*/
	
	if (!_waterShader->Render(_direct3D->GetDeviceContext(), _models[3].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, reflectionViewMatrix,
		_renderTextures[1].GetShaderResourceView(), _renderTextures[0].GetShaderResourceView(),
		_textures[3].GetTexture(), _waterTranslation, 0.01f)) {
		return false;
	}

	_direct3D->DisableAlphaBlending();

	_direct3D->TurnZBufferOff();

	if (!_sprite->Render(_direct3D->GetDeviceContext())) return false;

	_direct3D->EnableAlphaBlending();

	_direct3D->GetWorldMatrix(worldMatrix);

	for (int i = 0; i < _stringCount; i++) {
		_textStrings[i].Render(_direct3D->GetDeviceContext());
		if (!_fontShader->Render(_direct3D->GetDeviceContext(), _textStrings[i].GetIndexCount(),
			worldMatrix, _baseViewMatrix, orthoMatrix, _font->GetTexture(), _textStrings[i].GetPixelColor()))
			return false;
	}

	_direct3D->TurnZBufferOn();
	_direct3D->DisableAlphaBlending();
	_direct3D->EndScene();

	return true;
}

bool ApplicationClass::RenderRefractionToTexture()
{
	XMFLOAT4 clipPlane = XMFLOAT4(0.f, -1.f, 0.f, _waterHeight  + 0.1f);

	_renderTextures[0].SetRenderTarget(_direct3D->GetDeviceContext());
	_renderTextures[0].ClearRenderTarget(_direct3D->GetDeviceContext(), 0.f, 0.f, 0.f, 1.f);

	_camera->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	_direct3D->GetWorldMatrix(worldMatrix);
	_camera->GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixTranslation(0.f, 2.f, 0.f);

	_models[2].Render(_direct3D->GetDeviceContext());

	if (!_refractionShader->Render(_direct3D->GetDeviceContext(), _models[2].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[2].GetTexture(),
		_lights[3].GetDirection(), _lights[3].GetAmbientColor(), _lights[3].GetDiffuseColor(), clipPlane))
		return false;
	
	_direct3D->SetBackBufferRenderTarget();
	_direct3D->ResetViewport();
	
	return true;
}

bool ApplicationClass::RenderReflectionToTexture()
{

	_renderTextures[1].SetRenderTarget(_direct3D->GetDeviceContext());
	_renderTextures[1].ClearRenderTarget(_direct3D->GetDeviceContext(), 0.f, 0.f, 0.f, 1.f);

	_camera->RenderRefleciton(_waterHeight);

	XMMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix;

	_camera->GetReflectionViewMatrix(reflectionViewMatrix);
	_direct3D->GetWorldMatrix(worldMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixTranslation(0.f, 6.f, 8.f);

	_models[1].Render(_direct3D->GetDeviceContext());

	if (!_shaderManager->RenderTextureShader(_direct3D->GetDeviceContext(), _models[1].GetIndexCount(),
		worldMatrix, reflectionViewMatrix, projectionMatrix, _textures[1].GetTexture())) {
		return false;
	}

	_direct3D->SetBackBufferRenderTarget();
	_direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderSceneToTexture()
{
	_renderTextures[0].SetRenderTarget(_direct3D->GetDeviceContext());
	_renderTextures[0].ClearRenderTarget(_direct3D->GetDeviceContext(), 0.f, 0.5f, 1.f, 1.f);

	_camera->SetPosition(0.f, 0.f, -5.f);
	_camera->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	_direct3D->GetWorldMatrix(worldMatrix);
	_camera->GetViewMatrix(viewMatrix);
	_renderTextures[1].GetProjectionMatrix(projectionMatrix);

	_models[0].Render(_direct3D->GetDeviceContext());

	if (!_shaderManager->RenderTextureShader(_direct3D->GetDeviceContext(), _models[1].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[1].GetTexture())) {
		return false;
	}

	_direct3D->SetBackBufferRenderTarget();
	_direct3D->ResetViewport();

	return true;
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

bool ApplicationClass::UpdateRenderCountString(int renderCount)
{
	char tempString[16];
	sprintf_s(tempString, "%d", renderCount);

	char finalString[32];

	strcpy_s(finalString, "Render Count:");
	strcat_s(finalString, tempString);
	
	if (!_textStrings[0].UpdateText(_direct3D->GetDeviceContext(), _font.get(),
		finalString, 10, 10, 1.0f, 1.0f, 1.f))
		return false;

	return true;
}
