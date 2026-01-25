#include "ApplicationClass.hpp"

#include "TimerClass.hpp"

#include "DirectX11/InputClass.hpp"
#include "DirectX11/Object/CameraClass.hpp"
#include "DirectX11/Object/LightClass.hpp"

#include "DirectX11/Shader/Particle/ParticleShaderClass.hpp"
#include "DirectX11/Shader/Particle/ParticleSystemClass.hpp"

#include "DirectX11/Data/ModelClass.hpp"
#include "DirectX11/Data/TextureClass.hpp"
#include "DirectX11/Data/SimpleModelClass.hpp"
#include "DirectX11/Data/SpriteClass.hpp"
#include "DirectX11/Data/FontClass.hpp"
#include "DirectX11/Data/TextClass.hpp"
#include "DirectX11/Data/DeferredBuffersClass.hpp"

#include "DirectX11/Shader/Deferred/DeferredShaderClass.hpp"

#include "DirectX11/ShaderManager.hpp"
#include "DirectX11/Shader/Color/ColorShaderClass.hpp"
#include "DirectX11/Shader/Texture/TextureShaderClass.hpp"
#include "DirectX11/Shader/Texture/MultiTextureShaderClass.hpp"
#include "DirectX11/Shader/AlphaMap/AlphaMapShaderClass.hpp"
#include "DirectX11/Shader/NormalMap/NormalMapShaderClass.hpp"
#include "DirectX11/Shader/SpecMap/SpecMapShaderClass.hpp"
#include "DirectX11/Shader/LightMap/LightMapShaderClass.hpp"
#include "DirectX11/Shader/Light/LightShaderClass.hpp"
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
#include "DirectX11/Shader/Blur/BlurShaderClass.hpp"
#include "DirectX11/Shader/Fade/FadeShaderClass.hpp"
#include "DirectX11/Shader/Projection/ProjectionShaderClass.hpp"
#include "DirectX11/Shader/Glow/GlowShaderClass.hpp"

#include "DirectX11/Shader/Shadow/ShadowShaderClass.hpp"
#include "DirectX11/Shader/Shadow/MultiLIghtShadowShaderClass.hpp"
#include "DirectX11/Shader/Shadow/DirectionalLightShadowShaderClass.hpp"
#include "DirectX11/Shader/Shadow/SoftShadowShaderClass.hpp"

#include "DirectX11/Shader/Depth/DepthShaderClass.hpp"
#include "DirectX11/Shader/Depth/TransparentDepthShaderClass.hpp"

#include "DirectX11/Data/OrthoWindowClass.hpp"
#include "DirectX11/Shader/Blur/BlurClass.hpp"

#include "DirectX11/FrustumClass.hpp"
#include "DirectX11/PositionClass.hpp"
#include "DirectX11/ModelListClass.hpp"

#include "DirectX11/Data/RenderTextureClass.hpp"
#include "DirectX11/Data/DisplayPlaneClass.hpp"

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

	float aspectRatio = (float)screenWidth / (float)screenHeight;
	_cameras[1].SetPosition(0.f, 0.f, -10.f);
	_cameras[1].SetRotation(30.f, 0.f, 0.f);
	_cameras[1].SetProjectionParameters(3.141592 / 2.f, aspectRatio, SCREEN_DEPTH, SCREEN_NEAR);

	_cameras[2].SetPosition(2.f, 5.f, -2.f);
	_cameras[2].SetLookAt(0.f, 0.f, 0.f);
	_cameras[2].SetProjectionParameters(3.141592 / 2.f, 1.f, 1.f, 100.f);

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_numModels = 3;
	_models = std::make_unique<ModelClass[]>(_numModels);

	char modelFilename[3][128]{};
	strcpy_s(modelFilename[0], "./Assets/cube.txt");
	strcpy_s(modelFilename[1], "./Assets/trunk001.txt");
	strcpy_s(modelFilename[2], "./Assets/leaf001.txt");

	for (int i = 0; i < _numModels; i++) {
		if (!_models[i].Initialize(_direct3D->GetDevice(),
			_direct3D->GetDeviceContext(), modelFilename[i])) {
			MessageBox(hwnd, "Could not Initilalize the model object", modelFilename[i], MB_OK);
			return false;
		}
	}

	_textures = make_unique<TextureClass[]>(3);
	
	char textureFilename[3][128]{};
	strcpy_s(textureFilename[0], "./Assets/stone01.tga");
	strcpy_s(textureFilename[1], "./Assets/glowmap001.tga");
	strcpy_s(textureFilename[2], "./Assets/leaf001.tga");

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

	_numLights = 2;

	_lights = make_unique<LightClass[]>(_numLights);

	_lights[0].SetAmbientColor(0.15f, 0.15f, 0.15f, 1.f);
	_lights[0].SetDiffuseColor(1.0f, 1.f, 1.f, 1.f);
	_lights[0].SetPosition(5.f, 8.f, -5.f);
	_lights[0].SetOrthoParameters(20.f, 1.f, 50.f);
	_lights[0].SetLookAt(0, 0, 0);
	_lights[0].SetProjectionParameters(3.141592 / 2.f, 1.f, SCREEN_NEAR, SCREEN_DEPTH);
	_lights[0].SetDirection(1.0f, -1.0f, 0.2f);

	_lights[1].SetAmbientColor(0.15f, 0.15f, 0.15f, 1.f);
	_lights[1].SetDiffuseColor(1.0f, 1.f, 1.f, 1.f);
	_lights[1].SetPosition(-5.f, 8.f, -5.f);
	_lights[1].SetProjectionParameters(3.141592 / 2.f, 1.f, SCREEN_NEAR, SCREEN_DEPTH);

	_lights[1].SetLookAt(0, 0, 0);

	_particleSystem = make_unique<ParticleSystemClass>();
	
	if (!_particleSystem->Initialize(_direct3D->GetDevice(),
		_direct3D->GetDeviceContext(), textureFilename[0])) {
		return false;
	}

	_font = make_unique<FontClass>();
	if (!_font->Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), 0)) {
		MessageBox(hwnd, "Error", "Error", MB_OK);
		return false;
	}

	_particleShader = make_unique<ParticleShaderClass>();

	if (!_particleShader->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}
	_shaderManager = make_unique<ShaderManagerClass>();

	if (!_shaderManager->Initialize(_direct3D->GetDevice(), hwnd)) {
		return false;
	}

	_deferredBuffers = make_unique<DeferredBuffersClass>();
	if (!_deferredBuffers->Initialize(_direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR)) {
		MessageBox(hwnd, "Error", "Error", MB_OK);
		return false;
	}

	if (!CreateShader<DeferredShaderClass>(hwnd, _deferredShader)) {
		return false;
	}

	if (!CreateShader<LightShaderClass>(hwnd, _lightShader)) {
		return false;
	}

	if (!CreateShader<FontShaderClass>(hwnd, _fontShader)) {
		return false;
	}

	if (!CreateShader<TextureShaderClass>(hwnd, _textureShader)) {
		return false;
	}

	if (!CreateShader<FogShaderClass>(hwnd, _fogShader)) {
		return false;
	}

	if (!CreateShader<ClipPlaneShaderClass>(hwnd, _clipPlaneShader)) {
		return false;
	}

	if (!CreateShader<TranslateShaderClass>(hwnd, _translateShader)) {
		return false;
	}

	if (!CreateShader<TransparentShaderClass>(hwnd, _transparentShader)) {
		return false;
	}

	if (!CreateShader<ReflectionShaderClass>(hwnd, _reflectionShader)) {
		return false;
	}

	if (!CreateShader<WaterShaderClass>(hwnd, _waterShader)) {
		return false;
	}

	if (!CreateShader<RefractionShaderClass>(hwnd, _refractionShader)) {
		return false;
	}

	if (!CreateShader<GlassShaderClass>(hwnd, _glassShader)) {
		return false;
	}

	if (!CreateShader<FireShaderClass>(hwnd, _fireShader)) {
		return false;
	}

	if (!CreateShader<FadeShaderClass>(hwnd, _fadeShader)) {
		return false;
	}

	if (!CreateShader<ProjectionShaderClass>(hwnd, _projectionShader)) {
		return false;
	}

	if (!CreateShader<GlowShaderClass>(hwnd, _glowShader)) {
		return false;
	}
	/*
	*/
	if (!CreateShader<ShadowShaderClass>(hwnd, _shadowShader)) {
		return false;
	}

	if (!CreateShader<MultiLightShadowShaderClass>(hwnd, _mShadowShader)) {
		return false;
	}

	if (!CreateShader<DirectionalLightShadowShaderClass>(hwnd, _dShadowShader)) {
		return false;
	}

	if (!CreateShader<SoftShadowShaderClass>(hwnd, _sShadowShader)) {
		return false;
	}
	
	if (!CreateShader<DepthShaderClass>(hwnd, _depthShader)) {
		return false;
	}

	if (!CreateShader<TransparentDepthShaderClass>(hwnd, _transparentDepthShader)) {
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

	_renderTextures = make_unique<RenderTextureClass[]>(3);

	for (int i = 0; i < 3; i++) {
		if (!_renderTextures[i].Initialize(_direct3D->GetDevice(),
			screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1)) {
			return false;
		}
	}

	_depthTextures = make_unique<RenderTextureClass[]>(_numLights);

	for (int i = 0; i < _numLights; i++) {
		if (!_depthTextures[i].Initialize(_direct3D->GetDevice(),
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

	static float lightAngle = 270.f;
	static float lightPosX = 9.f;

	float frameTime = 10.0f;

	// Update the position of the light each frame.
	lightPosX -= 0.003f * frameTime;

	// Update the angle of the light each frame.
	lightAngle -= 0.03f * frameTime;

	if (lightAngle < 90.0f)
	{
		lightAngle = 270.0f;

		// Reset the light position also.
		lightPosX = 9.0f;
	}

	float radians = lightAngle * 0.0174532925f;

	/*
	// Update the direction of the light.
	_lights[0].SetDirection(sinf(radians), cosf(radians), 0.0f);
	
	// Set the position and lookat for the light.
	_lights[0].SetPosition(lightPosX, 8.0f, -0.1f);
	_lights[0].SetLookAt(-lightPosX, 0.0f, 0.0f);*/

	input.GetMouseLocation(mouseX, mouseY);

	char testString[32];

	if (TestIntersection(mouseX, mouseY)) {
		strcpy_s(testString, "Intersection: YES");
	}
	else
	{
		strcpy_s(testString, "Intersection: NO");
	}

	if (!UpdateMouseStrings(mouseX, mouseY, mouseDown)) return false;

	if (!_textStrings[2].UpdateText(_direct3D->GetDeviceContext(), _font.get(),
		testString, 10, 30, 0.f, 1.f, 0.f)) {
		return false;
	}

	if (!RenderSceneToTexture(1)) {
		return false;
	}

	return Render();

	if (!RenderGlowToTexture(2)) {
		return false;
	}

	if (!_blur->BlurTexture(_direct3D.get(),
		&_cameras[0], &_renderTextures[2],
		_textureShader.get(), _blurShader.get())) {
		return false;
	}

	//return PostProcess(2);

	if (!RenderDepthToTexture()) {
		return false;
	}

	if (!RenderBlackAndWhilteShadows(1)) {
		return false;
	}

	return Render();

	//return PostProcess(1);


	if (!_blur->BlurTexture(_direct3D.get(), &_cameras[0],
		&_renderTextures[1], _textureShader.get(), _blurShader.get())) {
	
		return false;
	}


	/*
	if (!UpdateMouseStrings(mouseX, mouseY, mouseDown)) return false;
	/**/
}

bool ApplicationClass::PostProcess(int targetIdx)
{
	float fogColor = 0.f;

	_direct3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;

	_direct3D->GetWorldMatrix(worldMatrix);

	_cameras[0].GetViewMatrix(viewMatrix);
	_direct3D->GetOrthoMatrix(orthoMatrix);

	static float fade = 1.f;

	_fullScreenWindow->Render(_direct3D->GetDeviceContext());

	if (!_textureShader->Render(_direct3D->GetDeviceContext(),
		_fullScreenWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix,
		//_depthTextures[0].GetShaderResourceView()))
		_renderTextures[targetIdx].GetShaderResourceView()))
	{
		return false;
	}


	_direct3D->EndScene();

	return true;
}

bool ApplicationClass::RenderDepthToTexture()
{

	for (int i = 0; i < _numLights; i++) {

		_depthTextures[i].SetRenderTarget(_direct3D->GetDeviceContext());
		_depthTextures[i].ClearRenderTarget(_direct3D->GetDeviceContext(), 0.f, 0.f, 0.f, 1.f);

		XMMATRIX lightViewMatrix, lightProjectionMatrix;

		_lights[i].GetViewMatrix(lightViewMatrix);
		_lights[i].GetProjectionMatrix(lightProjectionMatrix);

		XMMATRIX scaleMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f);
		XMMATRIX translateMatrix = XMMatrixTranslation(0.f, 1.f, 0.f);
		XMMATRIX worldMatrix = XMMatrixMultiply(scaleMatrix, translateMatrix);

		_models[1].Render(_direct3D->GetDeviceContext());

		if (!_depthShader->Render(
			_direct3D->GetDeviceContext(), _models[1].GetIndexCount(),
			worldMatrix, lightViewMatrix, lightProjectionMatrix)) {
			return false;
		}

		_models[2].Render(_direct3D->GetDeviceContext());

		if (!_transparentDepthShader->Render(
			_direct3D->GetDeviceContext(), _models[2].GetIndexCount(),
			worldMatrix, lightViewMatrix, lightProjectionMatrix, _textures[2].GetTexture())) {
			return false;
		}
		/*
		*/

		scaleMatrix = XMMatrixScaling(2.f, 2.f, 2.f);
		translateMatrix = XMMatrixTranslation(0.f, 1.f, 0.f);
		worldMatrix = XMMatrixMultiply(scaleMatrix, translateMatrix);

		_models[0].Render(_direct3D->GetDeviceContext());

		if (!_depthShader->Render(
			_direct3D->GetDeviceContext(), _models[0].GetIndexCount(),
			worldMatrix, lightViewMatrix, lightProjectionMatrix)) {
			return false;
		}
		
	}

	_direct3D->SetBackBufferRenderTarget();
	_direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderBlackAndWhilteShadows(int targetIdx)
{

	_renderTextures[targetIdx].SetRenderTarget(_direct3D->GetDeviceContext());
	_renderTextures[targetIdx].ClearRenderTarget(_direct3D->GetDeviceContext(), 1.f, 1.f, 1.f, 1.f);

	XMMATRIX viewMatrix, projectionMatrix;

	_cameras[1].GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);

	XMMATRIX lightViewMatrix, lightProjectionMatrix;

	_lights[0].GetViewMatrix(lightViewMatrix);
	_lights[0].GetProjectionMatrix(lightProjectionMatrix);

	XMMATRIX scaleMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f);
	XMMATRIX translateMatrix = XMMatrixTranslation(0.f, 1.f, 0.f);
	XMMATRIX worldMatrix = XMMatrixMultiply(scaleMatrix, translateMatrix);

	_models[1].Render(_direct3D->GetDeviceContext());

	if (!_shadowShader->Render(
		_direct3D->GetDeviceContext(), _models[1].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,
		_textures[1].GetTexture(), _depthTextures[0].GetShaderResourceView(),
		_lights[0].GetPosition(), 0.0022f)) {
		return false;
	}

	_models[2].Render(_direct3D->GetDeviceContext());

	if (!_shadowShader->Render(
		_direct3D->GetDeviceContext(), _models[2].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,
		_textures[2].GetTexture(), _depthTextures[0].GetShaderResourceView(),
		_lights[0].GetPosition(), 0.0022f)) {
		return false;
	}

	scaleMatrix = XMMatrixScaling(2.f, 2.f, 2.f);
	translateMatrix = XMMatrixTranslation(0.f, 1.f, 0.f);
	worldMatrix = XMMatrixMultiply(scaleMatrix, translateMatrix);

	_models[0].Render(_direct3D->GetDeviceContext());

	if (!_shadowShader->Render(
		_direct3D->GetDeviceContext(), _models[0].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix,
		_textures[0].GetTexture(), _depthTextures[0].GetShaderResourceView(),
		_lights[0].GetPosition(), 0.0022f)) {
		return false;
	}

	_direct3D->SetBackBufferRenderTarget();
	_direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMMATRIX baseViewMatrix, orthoMatrix;

	_direct3D->BeginScene(0.f, 0.5f, 0.8f, 1.f);

	_direct3D->GetWorldMatrix(worldMatrix);
	_cameras[1].GetViewMatrix(viewMatrix);
	_cameras[0].GetViewMatrix(baseViewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);
	_direct3D->GetOrthoMatrix(orthoMatrix);

	_direct3D->TurnZBufferOff();
	_fullScreenWindow->Render(_direct3D->GetDeviceContext());

	/*
	if (!_textureShader->Render(_direct3D->GetDeviceContext(), _fullScreenWindow->GetIndexCount(),
		worldMatrix, baseViewMatrix, projectionMatrix, _deferredBuffers->GetShaderResourceView(0))) {
		return false;
	}
	*/

	if (!_lightShader->Render(_direct3D->GetDeviceContext(), _fullScreenWindow->GetIndexCount(),
		worldMatrix, baseViewMatrix, orthoMatrix,
		_deferredBuffers->GetShaderResourceView(0), _deferredBuffers->GetShaderResourceView(1),
		_lights[0].GetDirection())) {
		return false;
	}

	_direct3D->TurnZBufferOn();
	_direct3D->EndScene();

	return true;

	_models[0].Render(_direct3D->GetDeviceContext());

	if (!_textureShader->Render(_direct3D->GetDeviceContext(), _models[0].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[0].GetTexture())) {
		return false;
	}

	_direct3D->EnableAlphaBlending();
	_direct3D->TurnZBufferOff();

	for (int i = 0; i < 3; i++) {

		_textStrings[i].Render(_direct3D->GetDeviceContext());
		if (!_fontShader->Render(_direct3D->GetDeviceContext(),
			_textStrings[i].GetIndexCount(),
			worldMatrix, baseViewMatrix, orthoMatrix, _font->GetTexture(), _textStrings[i].GetPixelColor()))
		{
			return false;
		}
		
	}
	_direct3D->TurnZBufferOn();
	_direct3D->DisableAlphaBlending();
	_direct3D->EndScene();

	return true;

	_renderTextures[0].SetRenderTarget(_direct3D->GetDeviceContext());
	_renderTextures[0].ClearRenderTarget(_direct3D->GetDeviceContext(), 0.f, 0.2f, 0.8f, 1.f);

	XMMATRIX lightViewMatrix, lightProjectionMatrix;
	XMMATRIX lightViewMatrix2, lightProjectionMatrix2;

	_direct3D->GetWorldMatrix(worldMatrix);
	_cameras[1].GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);

	_lights[0].GetViewMatrix(lightViewMatrix);
	_lights[0].GetProjectionMatrix(lightProjectionMatrix);
	_lights[1].GetViewMatrix(lightViewMatrix2);
	_lights[1].GetProjectionMatrix(lightProjectionMatrix2);


	XMMATRIX scaleMatrix = XMMatrixScaling(2.f, 2.f, 2.f);
	XMMATRIX translateMatrix = XMMatrixTranslation(0.f, 1.f, 0.f);
	worldMatrix = XMMatrixMultiply(scaleMatrix, translateMatrix);

	_models[0].Render(_direct3D->GetDeviceContext());

	if (!_sShadowShader->Render(
		_direct3D->GetDeviceContext(), _models[0].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix,
		lightViewMatrix, lightProjectionMatrix,
		_textures[0].GetTexture(), _renderTextures[1].GetShaderResourceView(),
		_lights[0].GetAmbientColor(), _lights[0].GetDiffuseColor(),
		_lights[0].GetPosition(), 0.0022f)) {
		return false;
	}

	scaleMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f);
	translateMatrix = XMMatrixTranslation(0.f, 1.f, 0.f);
	worldMatrix = XMMatrixMultiply(scaleMatrix, translateMatrix);

	_models[1].Render(_direct3D->GetDeviceContext());

	if (!_sShadowShader->Render(
		_direct3D->GetDeviceContext(), _models[1].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix,
		lightViewMatrix, lightProjectionMatrix,
		_textures[1].GetTexture(), _renderTextures[1].GetShaderResourceView(),
		_lights[0].GetAmbientColor(), _lights[0].GetDiffuseColor(),
		_lights[0].GetPosition(), 0.0022f)) {

		return false;
	}

	_direct3D->EnableAlphaBlending();

	_models[2].Render(_direct3D->GetDeviceContext());

	if (!_sShadowShader->Render(
		_direct3D->GetDeviceContext(), _models[2].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix,
		lightViewMatrix, lightProjectionMatrix,
		_textures[2].GetTexture(), _renderTextures[1].GetShaderResourceView(),
		_lights[0].GetAmbientColor(), _lights[0].GetDiffuseColor(),
		_lights[0].GetPosition(), 0.0022f)) {
		return false;
	}
	_direct3D->DisableAlphaBlending();


	_direct3D->SetBackBufferRenderTarget();
	_direct3D->ResetViewport();

	return PostProcess(0);

	/*
	if (!_shaderManager->RenderTextureShader(_direct3D->GetDeviceContext(), _models[0].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[1].GetTexture())) {
		return false;
	}

	/*
	*/

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
}

bool ApplicationClass::RenderSceneToTexture(int target)
{
	_deferredBuffers->SetRenderTargets(_direct3D->GetDeviceContext());
	_deferredBuffers->ClearRenderTargets(_direct3D->GetDeviceContext(), 0.f, 0.f, 0.f, 1.f);


	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	_direct3D->GetWorldMatrix(worldMatrix);
	_cameras[1].GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);

	_models[0].Render(_direct3D->GetDeviceContext());

	if (!_deferredShader->Render(_direct3D->GetDeviceContext(), _models[0].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[0].GetTexture())) {
	
		return false;
	}

	_direct3D->SetBackBufferRenderTarget();
	_direct3D->ResetViewport();

	return true;

}

bool ApplicationClass::RenderGlowToTexture(int target)
{
	_renderTextures[target].SetRenderTarget(_direct3D->GetDeviceContext());
	_renderTextures[target].ClearRenderTarget(_direct3D->GetDeviceContext(), 0.f, 0.f, 0.f, 1.f);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	_direct3D->GetWorldMatrix(worldMatrix);
	_cameras[1].GetViewMatrix(viewMatrix);
	_direct3D->GetProjectionMatrix(projectionMatrix);

	_models[0].Render(_direct3D->GetDeviceContext());

	if (!_textureShader->Render(_direct3D->GetDeviceContext(), _models[0].GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, _textures[1].GetTexture())) {

		return false;
	}

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

bool ApplicationClass::TestIntersection(int mouseX, int mouseY)
{
	XMMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, inverseWorldMatrix;
	XMFLOAT4X4 pMatrix, iViewMatrix;
	XMVECTOR direction, origin, rayOrigin, rayDirection;
	XMFLOAT3 cameraDirection, cameraOrigin, rayOri, rayDir;

	float pointX = ((2.f * (float)mouseX) / (float)_screenWidth) - 1.f;
	float pointY = (((2.f * (float)mouseY) / (float)_screenHeight) - 1.f) * -1.f;

	_direct3D->GetProjectionMatrix(projectionMatrix);
	XMStoreFloat4x4(&pMatrix, projectionMatrix);
	pointX = pointX / pMatrix._11;
	pointY = pointY / pMatrix._22;

	_cameras[1].GetViewMatrix(viewMatrix);
	inverseViewMatrix = XMMatrixInverse(NULL, viewMatrix);
	XMStoreFloat4x4(&iViewMatrix, inverseViewMatrix);

	cameraDirection.x = (pointX * iViewMatrix._11) + (pointY * iViewMatrix._21) + iViewMatrix._31;
	cameraDirection.y = (pointX * iViewMatrix._12) + (pointY * iViewMatrix._22) + iViewMatrix._32;
	cameraDirection.z = (pointX * iViewMatrix._13) + (pointY * iViewMatrix._23) + iViewMatrix._33;
	direction = XMLoadFloat3(&cameraDirection);

	cameraOrigin = _cameras[1].GetPosition();
	origin = XMLoadFloat3(&cameraOrigin);

	worldMatrix = XMMatrixTranslation(0.f, 0.f, 0.f);
	inverseWorldMatrix = XMMatrixInverse(nullptr, worldMatrix);

	rayOrigin = XMVector3TransformCoord(origin, inverseWorldMatrix);
	rayDirection = XMVector3TransformNormal(direction, inverseWorldMatrix);

	rayDirection = XMVector3Normalize(rayDirection);

	XMStoreFloat3(&rayOri, rayOrigin);
	XMStoreFloat3(&rayDir, rayDirection);

	return RaySphereIntersect(rayOri, rayDir, 2.f);
}

bool ApplicationClass::RaySphereIntersect(XMFLOAT3 rayOrigin, XMFLOAT3 rayDirection, float radius)
{
	float a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	float b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.f;
	float c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	float discriminant = (b * b) - (4 * a * c);

	if (discriminant < 0.f) {
		return false;
	}
	
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
