#include <windows.h>
#include <memory>
#include <directxmath.h>

#include "DirectX11/D3DClass.hpp"

using namespace std;
using namespace DirectX;

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.f;

class D3DClass;
class CameraClass;
class SimpleModelClass;
class ModelClass;
class SpriteClass;
class ColorShaderClass;
class TextureShaderClass;
class MultiTextureShaderClass;
class AlphaMapShaderClass;
class SpecMapShaderClass;
class LightMapShaderClass;
class LightShaderClass;
class FontShaderClass;
class FogShaderClass;
class ClipPlaneShaderClass;
class TranslateShaderClass;
class TransparentShaderClass;
class ReflectionShaderClass;
class WaterShaderClass;
class RefractionShaderClass;
class GlassShaderClass;
class FireShaderClass;
class DepthShaderClass;
class FadeShaderClass;
class ProjectionShaderClass;
class ShadowShaderClass;
class MultiLightShadowShaderClass;
class DirectionalLightShadowShaderClass;
class SoftShadowShaderClass;

class LightClass;
class FontClass;
class TextClass;
class InputClass;
class TextureClass;

class TimerClass;
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

class ApplicationClass {
private:
	std::unique_ptr<D3DClass> _direct3D;
	std::unique_ptr<CameraClass[]> _cameras;

	std::unique_ptr<TimerClass> _timer;

	std::unique_ptr<ParticleSystemClass> _particleSystem;
	std::unique_ptr<ParticleShaderClass> _particleShader;
	
	std::unique_ptr<ModelClass[]> _models;
	std::unique_ptr<SimpleModelClass> _simpleModel;

	std::unique_ptr<SpriteClass> _sprite;

	std::unique_ptr<ColorShaderClass> _colorShader;
	std::unique_ptr<TextureShaderClass> _textureShader;
	std::unique_ptr<MultiTextureShaderClass> _multiTextureShader;
	std::unique_ptr<AlphaMapShaderClass> _alphaMapShader;
	std::unique_ptr<SpecMapShaderClass> _specMapShader;
	std::unique_ptr<LightMapShaderClass> _lightMapShader;
	std::unique_ptr<LightShaderClass> _lightShader;
	std::unique_ptr<FogShaderClass> _fogShader;
	std::unique_ptr<ClipPlaneShaderClass> _clipPlaneShader;
	std::unique_ptr<TranslateShaderClass> _translateShader;
	std::unique_ptr<TransparentShaderClass> _transparentShader;
	std::unique_ptr<ReflectionShaderClass> _reflectionShader;
	std::unique_ptr<WaterShaderClass> _waterShader;
	std::unique_ptr<RefractionShaderClass> _refractionShader;
	std::unique_ptr<GlassShaderClass> _glassShader;
	std::unique_ptr<FireShaderClass> _fireShader;
	std::unique_ptr<DepthShaderClass> _depthShader;
	std::unique_ptr<FadeShaderClass> _fadeShader;
	std::unique_ptr<ProjectionShaderClass> _projectionShader;
	std::unique_ptr<ShadowShaderClass> _shadowShader;
	std::unique_ptr<MultiLightShadowShaderClass> _mShadowShader;
	std::unique_ptr<DirectionalLightShadowShaderClass> _dShadowShader;
	std::unique_ptr<SoftShadowShaderClass> _sShadowShader;

	std::unique_ptr<TextureClass[]> _textures;
	
	std::unique_ptr<FontShaderClass> _fontShader;

	std::unique_ptr<FontClass> _font;
	std::unique_ptr<TextClass[]> _textStrings;

	std::unique_ptr<LightClass[]> _lights;

	std::unique_ptr<ShaderManagerClass> _shaderManager;

	std::unique_ptr<PositionClass> _position;
	std::unique_ptr<FrustumClass> _frustum;
	std::unique_ptr<ModelListClass> _modelList;
	
	std::unique_ptr<RenderTextureClass[]> _renderTextures;
	std::unique_ptr<RenderTextureClass[]> _depthTextures;
	std::unique_ptr<DisplayPlaneClass> _displayPlane;

	std::unique_ptr<OrthoWindowClass> _fullScreenWindow;
	std::unique_ptr<BlurClass> _blur;
	std::unique_ptr<BlurShaderClass> _blurShader;
	
	int _numLights;
	int _stringCount;
	int _numModels;

	float _waterHeight;
	float _waterTranslation;

	XMMATRIX _baseViewMatrix;

public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(InputClass&);

private:
	bool PostProcess(int targetIdx);
	bool Render();
	bool RenderDepthToTexture();
	bool RenderBlackAndWhilteShadows(int targetIdx);
	bool RenderRefractionToTexture();
	bool RenderReflectionToTexture();

	template <typename T>
	bool CreateShader(HWND, std::unique_ptr<T>&);

	bool UpdateMouseStrings(int, int, bool);
	bool UpdateRenderCountString(int);
};

template<typename T>
bool ApplicationClass::CreateShader(HWND hwnd, std::unique_ptr<T>& ptr)
{
	ptr = make_unique<T>();
	if (!ptr->Initialize(_direct3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Error", "Error", MB_OK);
		return false;
	}
	return true;
}
