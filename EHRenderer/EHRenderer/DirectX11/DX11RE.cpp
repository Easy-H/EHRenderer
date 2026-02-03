#include "DX11RE.hpp"
#include <string.h>

#include "d3dclass.hpp"

#include "Object/CameraClass.hpp"
#include "Object/LightClass.hpp"

#include "Shader/Particle/ParticleShaderClass.hpp"
#include "Shader/Particle/ParticleSystemClass.hpp"
#include "Data/DeferredBuffersClass.hpp"

#include "Data/SimpleModelClass.hpp"
#include "Data/SpriteClass.hpp"
#include "Data/FontClass.hpp"
#include "Data/TextClass.hpp"

#include "ShaderManager.hpp"
#include "Shader/Color/ColorShaderClass.hpp"
#include "Shader/Texture/TextureShaderClass.hpp"
#include "Shader/Texture/MultiTextureShaderClass.hpp"
#include "Shader/AlphaMap/AlphaMapShaderClass.hpp"
#include "Shader/NormalMap/NormalMapShaderClass.hpp"
#include "Shader/SpecMap/SpecMapShaderClass.hpp"
#include "Shader/LightMap/LightMapShaderClass.hpp"
#include "Shader/Light/LightShaderClass.hpp"
#include "Shader/Font/FontShaderClass.hpp"
#include "Shader/Fog/FogShaderClass.hpp"
#include "Shader/ClipPlane/ClipPlaneShaderClass.hpp"
#include "Shader/Translate/TranslateShaderClass.hpp"
#include "Shader/Transparent/TransparentShaderClass.hpp"
#include "Shader/Reflection/ReflectionShaderClass.hpp"
#include "Shader/Water/WaterShaderClass.hpp"
#include "Shader/Refraction/RefractionShaderClass.hpp"
#include "Shader/Glass/GlassShaderClass.hpp"
#include "Shader/Fire/FireShaderClass.hpp"
#include "Shader/Fade/FadeShaderClass.hpp"
#include "Shader/Projection/ProjectionShaderClass.hpp"
#include "Shader/Glow/GlowShaderClass.hpp"
#include "Shader/Deferred/DeferredShaderClass.hpp"
#include "Shader/Blur/BlurClass.hpp"
#include "Shader/Blur/BlurShaderClass.hpp"
#include "Data/DisplayPlaneClass.hpp"

#include "Shader/GBuffer/GBufferShaderClass.hpp"
#include "Shader/Ssao/SsaoShaderClass.hpp"
#include "Shader/Ssao/SsaoBlurShaderClass.hpp"

#include "Shader/Shadow/ShadowShaderClass.hpp"
#include "Shader/Shadow/MultiLIghtShadowShaderClass.hpp"
#include "Shader/Shadow/DirectionalLightShadowShaderClass.hpp"
#include "Shader/Shadow/SoftShadowShaderClass.hpp"

#include "Shader/Depth/DepthShaderClass.hpp"
#include "Shader/Depth/TransparentDepthShaderClass.hpp"

#include "Data/OrthoWindowClass.hpp"

#include "FrustumClass.hpp"
#include "PositionClass.hpp"
#include "ModelListClass.hpp"

#include "Data/RenderTextureClass.hpp"


DX11RE::DX11RE()
{
}

DX11RE::DX11RE(const DX11RE*)
{
}

DX11RE::~DX11RE()
{
}

void DX11RE::GetView(XMMATRIX& viewMatrix)
{
	//_camera->GetViewMatrix(viewMatrix);
}

void DX11RE::GetProjection(XMMATRIX& projectionMatrix)
{
	_direct3D->GetProjectionMatrix(projectionMatrix);
}

ID3D11Device* DX11RE::GetDevice()
{
	return _direct3D->GetDevice();
}

ID3D11DeviceContext* DX11RE::GetDeviceContext()
{
	return _direct3D->GetDeviceContext();
}

bool DX11RE::Initialize(int screenWidth, int screenHeight)
{
	_direct3D = std::make_unique<D3DClass>();

	_direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, _hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	//_camera = std::make_unique<CameraClass>();

	float aspectRatio = (float)screenWidth / (float)screenHeight;

	//_camera->SetPosition(0.f, 0.f, -10.f);
	//_camera->SetRotation(30.f, 0.f, 0.f);
	//_camera->SetProjectionParameters(3.141592 / 2.f, aspectRatio, SCREEN_DEPTH, SCREEN_NEAR);

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

	_particleShader = make_unique<ParticleShaderClass>();

	if (!_particleShader->Initialize(_direct3D->GetDevice(), _hwnd)) {
		return false;
	}
	_shaderManager = make_unique<ShaderManagerClass>();

	if (!_shaderManager->Initialize(_direct3D->GetDevice(), _hwnd)) {
		return false;
	}

	_deferredBuffers = make_unique<DeferredBuffersClass>();
	if (!_deferredBuffers->Initialize(_direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR)) {
		MessageBox(_hwnd, "Error", "Error", MB_OK);
		return false;
	}

	if (!CreateShader<DeferredShaderClass>(_hwnd, _deferredShader)) {
		return false;
	}

	if (!CreateShader<LightShaderClass>(_hwnd, _lightShader)) {
		return false;
	}

	if (!CreateShader<FontShaderClass>(_hwnd, _fontShader)) {
		return false;
	}
	_textureShader = make_unique<TextureShaderClass>();

	if (!_textureShader->Initialize()) {
		return false;
	}

	if (!CreateShader<FogShaderClass>(_hwnd, _fogShader)) {
		return false;
	}

	if (!CreateShader<ClipPlaneShaderClass>(_hwnd, _clipPlaneShader)) {
		return false;
	}

	if (!CreateShader<TranslateShaderClass>(_hwnd, _translateShader)) {
		return false;
	}

	if (!CreateShader<TransparentShaderClass>(_hwnd, _transparentShader)) {
		return false;
	}

	if (!CreateShader<ReflectionShaderClass>(_hwnd, _reflectionShader)) {
		return false;
	}

	if (!CreateShader<WaterShaderClass>(_hwnd, _waterShader)) {
		return false;
	}

	if (!CreateShader<RefractionShaderClass>(_hwnd, _refractionShader)) {
		return false;
	}

	if (!CreateShader<GlassShaderClass>(_hwnd, _glassShader)) {
		return false;
	}

	if (!CreateShader<FireShaderClass>(_hwnd, _fireShader)) {
		return false;
	}

	if (!CreateShader<FadeShaderClass>(_hwnd, _fadeShader)) {
		return false;
	}

	if (!CreateShader<ProjectionShaderClass>(_hwnd, _projectionShader)) {
		return false;
	}

	if (!CreateShader<GlowShaderClass>(_hwnd, _glowShader)) {
		return false;
	}
	/*
	*/
	if (!CreateShader<ShadowShaderClass>(_hwnd, _shadowShader)) {
		return false;
	}

	if (!CreateShader<MultiLightShadowShaderClass>(_hwnd, _mShadowShader)) {
		return false;
	}

	if (!CreateShader<DirectionalLightShadowShaderClass>(_hwnd, _dShadowShader)) {
		return false;
	}

	if (!CreateShader<SoftShadowShaderClass>(_hwnd, _sShadowShader)) {
		return false;
	}

	if (!CreateShader<DepthShaderClass>(_hwnd, _depthShader)) {
		return false;
	}

	if (!CreateShader<TransparentDepthShaderClass>(_hwnd, _transparentDepthShader)) {
		return false;
	}

	return true;
}

int DX11RE::RegisterModel(const char* addr)
{
	return 0;

	/*
	if (_modelMap.contains(addr)) {

		return _modelMap[addr];
	}*/

	//_modelMap[addr] = _models.size();

	std::unique_ptr<ModelClass> model = std::make_unique<ModelClass>();

	model->Initialize(addr);

	//_models.push_back(model);

	//return _modelMap[addr];
}

int DX11RE::RegisterTexture(const char* addr)
{
	return 0;
	/*
	if (_textureMap.contains(addr)) {

		return _textureMap[addr];
	}
	*/
	//_textureMap[addr] = _textures.size();

	std::unique_ptr<TextureClass> texture = std::make_unique<TextureClass>();

	texture->Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), addr);

	//_textures.push_back(texture);

	//return _textureMap[addr];
}

int DX11RE::RegisterFont(const char* addr)
{
	return 0;
}

ShaderBaseBase* DX11RE::GetShader(const char* name)
{
	return _textureShader.get();
}

bool DX11RE::Render()
{
	return false;
}

ID3D11ShaderResourceView* DX11RE::GetTexture(int id)
{
	return nullptr;
	//return _textures[id]->GetTexture();
}