#ifndef _DX11RE_H_
#define _DX11RE_H_

#include "../../EasyH/Singleton.hpp"
#include "../ImporterBase.hpp"
#include "../RenderEngineBase.hpp"

#include "D3DClass.hpp"

#include <d3d11.h>
#include <unordered_map>
#include <vector>
#include <directxmath.h>

/*
class ModelClass;
class TextureClass;*/

#include "Data/ModelClass.hpp"
#include "Data/TextureClass.hpp"
#include "../Data/Material.hpp"

class LightClass;
class CameraClass;
class DeferredBuffersClass;

class SimpleModelClass;

class SpriteClass;

class ShaderBaseBase;
class DeferredShaderClass;
class ColorShaderClass;
class SpecMapShaderClass;
class LightMapShaderClass;
class LightShaderClass;
class FontShaderClass;
class FogShaderClass;
class TranslateShaderClass;
class TransparentShaderClass;
class ReflectionShaderClass;
class WaterShaderClass;
class RefractionShaderClass;
class GlassShaderClass;
class FireShaderClass;
class ProjectionShaderClass;
class ShadowShaderClass;
class MultiLightShadowShaderClass;
class DirectionalLightShadowShaderClass;
class SoftShadowShaderClass;
class GlowShaderClass;
class SsaoShaderClass;
class SsaoBlurShaderClass;
class GBufferShaderClass;

class TransparentDepthShaderClass;

class FontClass;
class TextClass;
class InputClass;

class FrustumClass;
class PositionClass;
class ModelListClass;

class ShaderManagerClass;

class RenderTextureClass;
class DisplayPlaneClass;

class BlurClass;
class OrthoWindowClass;
class BlurShaderClass;

class ParticleSystemClass;
class ParticleShaderClass;

using namespace DirectX;

const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.f;

class DX11RE : public RenderEngineBase, public EasyH::Singleton<DX11RE> {
private:
	class DX11RenderUnit {
	public:
		DX11RenderUnit();
		void Initialize(DX11RE* engine, const std::string& modelAddr, const std::string& materialAddr);
		bool Render(DX11RE* engine);
	private:
		Material* _material;
		int _modelId;
	};
public:
	DX11RE();
	DX11RE(const DX11RE*);
	~DX11RE();

	void SetHWND(HWND hwnd) { _hwnd = hwnd; }
	virtual bool Initialize(int screenWidth, int screenHeight, bool fullscreen) override;

	Material* RegisterMaterial(const std::string& addr);
	virtual int RegisterModel(const std::string& addr) override;
	virtual int RegisterTexture(const std::string& addr) override;
	virtual int RegisterFont(const std::string& addr) override;

	int BindModel(int id);

	virtual void RegisterRenderUnit(const std::string& modelAddr,
		const std::string& materialAddr, const Transform* transform) override;

	virtual ShaderBaseBase* GetShader(const std::string& name) override;

	virtual bool Render() override;

	HWND GetHWND() { return _hwnd; }

	void GetView(XMMATRIX& viewMatrix);
	void GetProjection(XMMATRIX& projectionMatrix);
	LightClass* GetLights(int idx) { return _lights[idx].get(); }

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11ShaderResourceView* GetTexture(int id);

private:
	bool CreateLegacyShaders();
	bool CreateShaders();

	template <typename T>
	bool CreateShader(HWND, std::unique_ptr<T>&);
	template <typename T>
	bool CreateLegacyShader(HWND, std::unique_ptr<T>&);

	std::vector<std::unique_ptr<DX11RenderUnit>> _renderTargets;

	std::unique_ptr<D3DClass> _direct3D;
	std::unique_ptr<CameraClass> _camera;

	std::unordered_map<std::string, unique_ptr<Material>> _materialMap;
	std::unordered_map<std::string, unique_ptr<ShaderBaseBase>> _shaderMap;

	std::unordered_map<std::string, int> _modelMap;
	std::vector<std::unique_ptr<ModelClass>> _models;

	std::unordered_map<std::string, int> _textureMap;
	std::vector<std::unique_ptr<TextureClass>> _textures;

	HWND _hwnd;

	int _screenWidth;
	int _screenHeight;

	int _numLights;

	std::unique_ptr<CameraClass[]> _cameras;

	std::unique_ptr<ParticleSystemClass> _particleSystem;
	std::unique_ptr<ParticleShaderClass> _particleShader;

	std::unique_ptr<SimpleModelClass> _simpleModel;

	std::unique_ptr<SpriteClass> _sprite;

	std::unique_ptr<ColorShaderClass> _colorShader;
	std::unique_ptr<SpecMapShaderClass> _specMapShader;
	std::unique_ptr<LightMapShaderClass> _lightMapShader;
	std::unique_ptr<LightShaderClass> _lightShader;
	std::unique_ptr<FogShaderClass> _fogShader;
	std::unique_ptr<TranslateShaderClass> _translateShader;
	std::unique_ptr<TransparentShaderClass> _transparentShader;
	std::unique_ptr<ReflectionShaderClass> _reflectionShader;
	std::unique_ptr<WaterShaderClass> _waterShader;
	std::unique_ptr<RefractionShaderClass> _refractionShader;
	std::unique_ptr<GlassShaderClass> _glassShader;
	std::unique_ptr<FireShaderClass> _fireShader;
	std::unique_ptr<ProjectionShaderClass> _projectionShader;
	std::unique_ptr<GlowShaderClass> _glowShader;

	std::unique_ptr<ShadowShaderClass> _shadowShader;
	std::unique_ptr<MultiLightShadowShaderClass> _mShadowShader;
	std::unique_ptr<DirectionalLightShadowShaderClass> _dShadowShader;
	std::unique_ptr<SoftShadowShaderClass> _sShadowShader;

	std::unique_ptr<SsaoBlurShaderClass> _ssaoBlurShader;
	std::unique_ptr<SsaoShaderClass> _ssaoShader;
	std::unique_ptr<GBufferShaderClass> _gBufferShader;

	std::unique_ptr<TransparentDepthShaderClass> _transparentDepthShader;

	std::unique_ptr<FontShaderClass> _fontShader;

	std::unique_ptr<FontClass> _font;
	//std::unique_ptr<TextClass[]> _textStrings;

	std::vector<std::unique_ptr<LightClass>> _lights;

	std::unique_ptr<PositionClass> _position;
	std::unique_ptr<FrustumClass> _frustum;
	std::unique_ptr<ModelListClass> _modelList;

	//std::unique_ptr<RenderTextureClass[]> _renderTextures;
	//std::unique_ptr<RenderTextureClass[]> _depthTextures;
	std::unique_ptr<DisplayPlaneClass> _displayPlane;

	std::unique_ptr<OrthoWindowClass> _fullScreenWindow;
	std::unique_ptr<BlurClass> _blur;
	std::unique_ptr<BlurShaderClass> _blurShader;

	std::unique_ptr<DeferredShaderClass> _deferredShader;
	std::unique_ptr<DeferredBuffersClass> _deferredBuffers;

};

template<typename T>
bool DX11RE::CreateShader(HWND hwnd, std::unique_ptr<T>& ptr)
{
	ptr = make_unique<T>();
	if (!ptr->Initialize(_direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Error", "Error", MB_OK);
		return false;
	}
	return true;
}


template<typename T>
bool DX11RE::CreateLegacyShader(HWND hwnd, std::unique_ptr<T>& ptr)
{
	ptr = make_unique<T>();
	if (!ptr->Initialize(_direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Error", "Error", MB_OK);
		return false;
	}
	return true;
}

#endif