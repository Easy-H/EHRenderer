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
	_camera->GetViewMatrix(viewMatrix);
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

bool DX11RE::Initialize(int screenWidth, int screenHeight, bool fullscreen)
{
	_direct3D = std::make_unique<D3DClass>();

	_direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, _hwnd, fullscreen, SCREEN_DEPTH, SCREEN_NEAR);

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_camera = std::make_unique<CameraClass>();

	float aspectRatio = (float)screenWidth / (float)screenHeight;

	_camera->SetPosition(0.f, 4.f, -6.f);
	_camera->SetRotation(30.f, 0.f, 0.f);
	_camera->SetProjectionParameters(3.141592 / 2.f, aspectRatio, SCREEN_DEPTH, SCREEN_NEAR);

	if (!CreateLegacyShaders()) {
		return false;
	}
	if (!CreateShaders()) {
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

	return true;
}

Material* DX11RE::RegisterMaterial(const char* addr)
{
	if (_materialMap.contains(addr)) {

		return _materialMap[addr].get();
	}

	std::unique_ptr<Material> material = std::make_unique<Material>();

	material->Initialize(this, addr);

	_materialMap[addr] = std::move(material);

	return _materialMap[addr].get();
}

int DX11RE::RegisterModel(const char* addr)
{
	if (_modelMap.contains(addr)) {

		return _modelMap[addr];
	}

	_modelMap[addr] = _models.size();

	std::unique_ptr<ModelClass> model = std::make_unique<ModelClass>();

	model->Initialize(addr);

	_models.push_back(std::move(model));

	return _modelMap[addr];
}

int DX11RE::RegisterTexture(const char* addr)
{
	if (_textureMap.contains(addr)) {

		return _textureMap[addr];
	}

	_textureMap[addr] = _textures.size();

	std::unique_ptr<TextureClass> texture = std::make_unique<TextureClass>();

	texture->Initialize(_direct3D->GetDevice(), _direct3D->GetDeviceContext(), addr);

	_textures.push_back(std::move(texture));

	return _textureMap[addr];
}

int DX11RE::RegisterFont(const char* addr)
{
	return 0;
}

int DX11RE::BindModel(int id)
{
	_models[id]->Render(_direct3D->GetDeviceContext());
	return _models[id]->GetIndexCount();
}

void DX11RE::RegisterRenderUnit(const char* modelAddr, const char* materialAddr, const Transform* transform)
{
	std::unique_ptr<DX11RenderUnit> unit = std::make_unique<DX11RenderUnit>();
	
	unit->Initialize(this, modelAddr, materialAddr);

	_renderTargets.push_back(std::move(unit));
}

ShaderBaseBase* DX11RE::GetShader(const char* name)
{
	return _textureShader.get();
}

bool DX11RE::Render()
{
	_direct3D->BeginScene(0.f, 0.f, 0.f, 1.f);

	_camera->Render();

	for (int i = 0; i < _renderTargets.size(); i++) {
		if (!_renderTargets[i]->Render(this))
			return false;
	}


	_direct3D->EndScene();

	return true;

}

ID3D11ShaderResourceView* DX11RE::GetTexture(int id)
{
	return _textures[id]->GetTexture();
}

bool DX11RE::CreateShaders()
{
	if (!CreateShader<TextureShaderClass>(_hwnd, _textureShader)) {
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

bool DX11RE::CreateLegacyShaders()
{

	if (!CreateLegacyShader<DeferredShaderClass>(_hwnd, _deferredShader)) {
		return false;
	}

	if (!CreateLegacyShader<LightShaderClass>(_hwnd, _lightShader)) {
		return false;
	}

	if (!CreateLegacyShader<FontShaderClass>(_hwnd, _fontShader)) {
		return false;
	}

	if (!CreateLegacyShader<FogShaderClass>(_hwnd, _fogShader)) {
		return false;
	}

	if (!CreateLegacyShader<ClipPlaneShaderClass>(_hwnd, _clipPlaneShader)) {
		return false;
	}

	if (!CreateLegacyShader<TranslateShaderClass>(_hwnd, _translateShader)) {
		return false;
	}

	if (!CreateLegacyShader<TransparentShaderClass>(_hwnd, _transparentShader)) {
		return false;
	}

	if (!CreateLegacyShader<ReflectionShaderClass>(_hwnd, _reflectionShader)) {
		return false;
	}

	if (!CreateLegacyShader<WaterShaderClass>(_hwnd, _waterShader)) {
		return false;
	}

	if (!CreateLegacyShader<RefractionShaderClass>(_hwnd, _refractionShader)) {
		return false;
	}

	if (!CreateLegacyShader<GlassShaderClass>(_hwnd, _glassShader)) {
		return false;
	}

	if (!CreateLegacyShader<FireShaderClass>(_hwnd, _fireShader)) {
		return false;
	}

	if (!CreateLegacyShader<FadeShaderClass>(_hwnd, _fadeShader)) {
		return false;
	}

	if (!CreateLegacyShader<ProjectionShaderClass>(_hwnd, _projectionShader)) {
		return false;
	}

	if (!CreateLegacyShader<GlowShaderClass>(_hwnd, _glowShader)) {
		return false;
	}
	/*
	*/
	if (!CreateLegacyShader<ShadowShaderClass>(_hwnd, _shadowShader)) {
		return false;
	}

	if (!CreateLegacyShader<MultiLightShadowShaderClass>(_hwnd, _mShadowShader)) {
		return false;
	}

	if (!CreateLegacyShader<DirectionalLightShadowShaderClass>(_hwnd, _dShadowShader)) {
		return false;
	}

	if (!CreateLegacyShader<SoftShadowShaderClass>(_hwnd, _sShadowShader)) {
		return false;
	}

	return true;

}

DX11RE::DX11RenderUnit::DX11RenderUnit()
{

}

void DX11RE::DX11RenderUnit::Initialize(DX11RE* engine, const char* modelAddr, const char* materialAddr)
{
	_modelId = engine->RegisterModel(modelAddr);
	_material = engine->RegisterMaterial(materialAddr);
}

bool DX11RE::DX11RenderUnit::Render(DX11RE* engine) {

	int indexCount = engine->BindModel(_modelId);

	_material->Render(engine, indexCount, nullptr);

	return true;
}