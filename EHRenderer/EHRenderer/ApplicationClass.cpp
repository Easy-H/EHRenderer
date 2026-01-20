#include "ApplicationClass.hpp"

#include "TimerClass.hpp"

#include "DirectX11/InputClass.hpp"
#include "DirectX11/D3DClass.hpp"
#include "DirectX11/CameraClass.hpp"

#include "DirectX11/Shader/Particle/ParticleShaderClass.hpp"
#include "DirectX11/Shader/Particle/ParticleSystemClass.hpp"

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
#include "DirectX11/Shader/Glass/GlassShaderClass.hpp"
#include "DirectX11/Shader/Fire/FireShaderClass.hpp"
#include "DirectX11/Shader/Depth/DepthShaderClass.hpp"
#include "DirectX11/Shader/Blur/BlurShaderClass.hpp"
#include "DirectX11/Shader/Fade/FadeShaderClass.hpp"
#include "DirectX11/Shader/Projection/ProjectionShaderClass.hpp"

#include "DirectX11/OrthoWindowClass.hpp"
#include "DirectX11/Shader/Blur/BlurClass.hpp"

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
	
	_cameras = std::make_unique<CameraClass[]>(3);

	_cameras[0].SetPosition(0.f, 0.f, -10.f);
	_cameras[0].GetViewMatrix(_baseViewMatrix);

	_cameras[1].SetPosition(0.f, 0.f, -10.f);
	_cameras[1].SetRotation(30.f, 0.f, 0.f);
	_cameras[1].SetProjectionParameters(3.141592 / 2.f, 1.f, SCREEN_DEPTH, SCREEN_NEAR);

	_cameras[2].SetPosition(2.f, 5.f, -2.f);
	_cameras[2].SetLookAt(0.f, 0.f, 0.f);
	_cameras[2].SetProjectionParameters(3.141592 / 2.f, 1.f, 1.f, 100.f);

	_models = std::make_unique<ModelClass[]>(4);

	char modelFilename[2][128]{};
	strcpy_s(modelFilename[0], "./Assets/plane01.txt");
	strcpy_s(modelFilename[1], "./Assets/cube.txt");

	for (int i = 0; i < 2; i++) {
		if (!_models[i].Initialize(_direct3D->GetDevice(),
			_direct3D->GetDeviceContext(), modelFilename[i])) {
			MessageBox(hwnd, "Could not Initilalize the model object", modelFilename[i], MB_OK);
			return false;
		}
	}

	_textures = make_unique<TextureClass[]>(3);
	
	char textureFilename[3][128]{};
	strcpy_s(textureFilename[0], "./Assets/metal001.tga");
	strcpy_s(textureFilename[1], "./Assets/stone01.tga");
	strcpy_s(textureFilename[2], "./Assets/grate.tga");

	for (int i = 0; i < 3; i++) {
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

	_numLights = 1;

	_lights = make_unique<LightClass[]>(_numLights);

	_lights[0].SetAmbientColor(0.15f, 0.15f, 0.15f, 1.f);
	_lights[0].SetDiffuseColor(1.0f, 1.f, 1.f, 1.f);
	_lights[0].SetPosition(2.f, 5.f, -2.f);

	_particleSystem = make_unique<ParticleSystemClass>();
	
	if (!_particleSystem->Initialize(_direct3D->GetDevice(),
		_direct3D->GetDeviceContext(), textureFilename[0])) {
		return false;
	}

	_particleShader = make_unique<ParticleShaderClass>();

	if (!_particleShader->Initialize(_direct3D->GetDevice(), hwnd)) {
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

	_shaderManager = make_unique<ShaderManagerClass>();

	if (!_shaderManager->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_textureShader = make_unique<TextureShaderClass>();

	if (!_textureShader->Initialize(_direct3D->GetDevice(), hwnd)) {
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

	_glassShader = make_unique<GlassShaderClass>();

	if (!_glassShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_fireShader = make_unique<FireShaderClass>();

	if (!_fireShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_depthShader = make_unique<DepthShaderClass>();

	if (!_depthShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_fadeShader = make_unique<FadeShaderClass>();

	if (!_fadeShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_projectionShader = make_unique<ProjectionShaderClass>();

	if (!_projectionShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Error", "Error", MB_OK);
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

	//_position->SetPosition(0.f, 0.f, -10.f);
	_position->SetPosition(0.f, 7.f, -10.f);
	_frustum = make_unique<FrustumClass>();

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

	_fullScreenWindow = make_unique<OrthoWindowClass>();

	if (!_fullScreenWindow->Initialize(_direct3D->GetDevice(), screenWidth, screenHeight))
	{
		return false;
	}

	_blur = make_unique<BlurClass>();

	int downSampleWidth = screenWidth / 2;
	int downSampleHeight = screenHeight / 2;

	if (!_blur->Initialize(_direct3D.get(), downSampleWidth, downSampleHeight, SCREEN_NEAR, SCREEN_DEPTH, screenWidth, screenHeight)) {
		return false;
	}

	_blurShader = make_unique<BlurShaderClass>();

	if (!_blurShader->Initialize(_direct3D->GetDevice(), hwnd)) {
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

	_position->MoveLeft(input.IsLeftArrowPressed());
	_position->MoveRight(input.IsRightArrowPressed());

	float x, y, z;

	_position->GetPosition(x, y, z);

	_cameras[1].SetPosition(x, y, z);

	_waterTranslation += 0.001f;
	if (_waterTranslation > 1.0f)
	{
		_waterTranslation -= 1.0f;
	}

	int mouseX, mouseY;
	input.GetMouseLocation(mouseX, mouseY);
	bool mouseDown = input.IsMousePressed();

	if (!_particleSystem->Frame(_timer->GetTime(), _direct3D->GetDeviceContext())) {
		return false;
	}

	if (!RenderSceneToTexture()) {
		return false;
	}
	/*
	if (!UpdateMouseStrings(mouseX, mouseY, mouseDown)) return false;

	if (!_blur->BlurTexture(_direct3D.get(), _camera.get(),
		&_renderTextures[0], _textureShader.get(), _blurShader.get())) {
		return false;
	}

	/**/

	return Render();
}

bool ApplicationClass::Render()
{

	float fogColor = 0.f;

	_direct3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	_direct3D->GetWorldMatrix(worldMatrix);
	
	_cameras[0].GetViewMatrix(viewMatrix);
	_cameras[0].GetProjectionMatrix(projectionMatrix);
	_direct3D->GetOrthoMatrix(orthoMatrix);

	static float fade = 1.f;

	//_models[0].Render(_direct3D->GetDeviceContext());
	_fullScreenWindow->Render(_direct3D->GetDeviceContext());

	if (!_textureShader->Render(_direct3D->GetDeviceContext(),
		_fullScreenWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		//_textures[0].GetTexture()))
		_renderTextures[0].GetShaderResourceView()))
	{
		return false;
	}

	/*
	if (!_blurShader->Render(_direct3D->GetDeviceContext(),
		_models[0].GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		_textures[0].GetTexture(), 800, 600, 0))
		//_renderTextures[0].GetShaderResourceView(), 800, 600, 0))
	{
		return false;
	}
	/*
	if (!_shaderManager->RenderTextureShader(_direct3D->GetDeviceContext(), _models[0].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[0].GetTexture())) {
		return false;
	}

	if (!_textureShader->Render(_direct3D->GetDeviceContext(),
		_fullScreenWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		//_textures[0].GetTexture()))
		_renderTextures[0].GetShaderResourceView()))
	{
		return false;
	}*/

	/*
	XMFLOAT3 cameraPosition = _camera->GetPosition();
	XMFLOAT3 modelPosition = XMFLOAT3(0.f, 1.5f, 0.f);

	float pi = 3.14159265358979323846f;
	float angle = atan2(modelPosition.x - cameraPosition.x,
		modelPosition.y - cameraPosition.z) * (180.f / pi);

	float rotation = (float)angle * 0.0174532925f;

	worldMatrix = XMMatrixRotationY(rotation);

	XMMATRIX translateMatrix = XMMatrixTranslation(
		modelPosition.x, modelPosition.y, modelPosition.z);

	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	_models[1].Render(_direct3D->GetDeviceContext());

	if (!_shaderManager->RenderTextureShader(_direct3D->GetDeviceContext(), _models[1].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[1].GetTexture())) {
		return false;
	}
	*/

	/*
	static float frameTime = 0.f;
	frameTime += 0.01f;

	if (frameTime > 1000.f) {
		frameTime -= 1000.f;
	}

	XMFLOAT3 scrollSpeeds = XMFLOAT3(1.3f, 2.1f, 2.3f);
	XMFLOAT3 scales = XMFLOAT3(1.f, 2.f, 3.f);

	XMFLOAT2 distortion1 = XMFLOAT2(0.1f, 0.2f);
	XMFLOAT2 distortion2 = XMFLOAT2(0.1f, 0.3f);
	XMFLOAT2 distortion3 = XMFLOAT2(0.1f, 0.1f);

	float distortionScale = 0.8f;
	float distortionBias = 0.5f;

	_direct3D->EnableAlphaBlending();

	_models[0].Render(_direct3D->GetDeviceContext());

	if (!_fireShader->Render(_direct3D->GetDeviceContext(), _models[0].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix,
		_textures[0].GetTexture(), _textures[1].GetTexture(), _textures[2].GetTexture(),
		frameTime, scrollSpeeds, scales, distortion1, distortion2, distortion3, distortionScale, distortionBias)) {
		return false;
	}

	_direct3D->DisableAlphaBlending();*/

	_direct3D->EndScene();

	return true;
}

bool ApplicationClass::RenderSceneToTexture()
{
	_renderTextures[0].SetRenderTarget(_direct3D->GetDeviceContext());
	_renderTextures[0].ClearRenderTarget(_direct3D->GetDeviceContext(), 0.f, 0.f, 0.f, 1.f);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	XMMATRIX viewMatrix2, projectionMatrix2;

	_direct3D->GetWorldMatrix(worldMatrix);
	_cameras[1].GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);

	_cameras[2].GetViewMatrix(viewMatrix2);
	_cameras[2].GetProjectionMatrix(projectionMatrix2);

	float brightness = 1.5f;

	_models[0].Render(_direct3D->GetDeviceContext());
	
	if (!_projectionShader->Render(_direct3D->GetDeviceContext(), _particleSystem->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, viewMatrix2, projectionMatrix2,
		_textures[0].GetTexture(), _textures[2].GetTexture(),
		_lights[0].GetAmbientColor(), _lights[0].GetDiffuseColor(),
		_lights[0].GetPosition(), brightness)) {
		return false;
	}

	worldMatrix = XMMatrixTranslation(0.0f, 2.0f, 0.0f);

	_models[1].Render(_direct3D->GetDeviceContext());

	if (!_projectionShader->Render(_direct3D->GetDeviceContext(), _particleSystem->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, viewMatrix2, projectionMatrix2,
		_textures[1].GetTexture(), _textures[2].GetTexture(),
		_lights[0].GetAmbientColor(), _lights[0].GetDiffuseColor(),
		_lights[0].GetPosition(), brightness)) {
		return false;
	}
	/*
	if (!_shaderManager->RenderTextureShader(_direct3D->GetDeviceContext(), _models[0].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[1].GetTexture())) {
		return false;
	}

	/*
	*/

	_direct3D->SetBackBufferRenderTarget();
	_direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderRefractionToTexture()
{
	XMFLOAT4 clipPlane = XMFLOAT4(0.f, -1.f, 0.f, _waterHeight  + 0.1f);

	_renderTextures[0].SetRenderTarget(_direct3D->GetDeviceContext());
	_renderTextures[0].ClearRenderTarget(_direct3D->GetDeviceContext(), 0.f, 0.f, 0.f, 1.f);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	_direct3D->GetWorldMatrix(worldMatrix);
	_cameras[0].GetViewMatrix(viewMatrix);
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

	_cameras[0].RenderRefleciton(_waterHeight);

	XMMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix;

	_cameras[0].GetReflectionViewMatrix(reflectionViewMatrix);
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
