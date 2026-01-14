#include <windows.h>
#include <memory>
#include <directxmath.h>

using namespace std;
using namespace DirectX;

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class D3DClass;
class CameraClass;
class SimpleModelClass;
class ModelClass;
class SpriteClass;
class ColorShaderClass;
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
class BlurShaderClass;

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

class ApplicationClass {
private:
	std::unique_ptr<D3DClass> _direct3D;
	std::unique_ptr<CameraClass> _camera;
	
	std::unique_ptr<ModelClass[]> _models;
	std::unique_ptr<SimpleModelClass> _simpleModel;

	std::unique_ptr<SpriteClass> _sprite;

	std::unique_ptr<ColorShaderClass> _colorShader;
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
	std::unique_ptr<BlurShaderClass> _blurShader;

	std::unique_ptr<TextureClass[]> _textures;
	
	std::unique_ptr<FontShaderClass> _fontShader;

	std::unique_ptr<FontClass> _font;
	std::unique_ptr<TextClass[]> _textStrings;

	std::unique_ptr<LightClass[]> _lights;

	std::unique_ptr<ShaderManagerClass> _shaderManager;

	std::unique_ptr<TimerClass> _timer;
	std::unique_ptr<PositionClass> _position;
	std::unique_ptr<FrustumClass> _frustum;
	std::unique_ptr<ModelListClass> _modelList;


	std::unique_ptr<RenderTextureClass[]> _renderTextures;
	std::unique_ptr<DisplayPlaneClass> _displayPlane;
	
	int _numLights;
	int _stringCount;

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
	bool Render();
	bool RenderRefractionToTexture();
	bool RenderReflectionToTexture();
	bool RenderSceneToTexture();

	bool UpdateMouseStrings(int, int, bool);
	bool UpdateRenderCountString(int);
};