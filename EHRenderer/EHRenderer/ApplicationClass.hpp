#include <windows.h>
#include <memory>

using namespace std;

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
class TextureShaderClass;
class MultiTextureShaderClass;
class AlphaMapShaderClass;
class NormalMapShaderClass;
class SpecMapShaderClass;
class LightMapShaderClass;
class LightShaderClass;
class LightClass;
class FontShaderClass;
class FontClass;
class TextClass;
class InputClass;
class TextureClass;

class ApplicationClass {
private:
	std::unique_ptr<D3DClass> _direct3D;
	std::unique_ptr<CameraClass> _camera;
	
	std::unique_ptr<ModelClass> _model;
	std::unique_ptr<SimpleModelClass> _simpleModel;

	std::unique_ptr<SpriteClass> _sprite;

	std::unique_ptr<ColorShaderClass> _colorShader;
	std::unique_ptr<TextureShaderClass> _textureShader;
	std::unique_ptr<MultiTextureShaderClass> _multiTextureShader;
	std::unique_ptr<AlphaMapShaderClass> _alphaMapShader;
	std::unique_ptr<NormalMapShaderClass> _normalMapShader;
	std::unique_ptr<SpecMapShaderClass> _specMapShader;
	std::unique_ptr<LightMapShaderClass> _lightMapShader;
	std::unique_ptr<LightShaderClass> _lightShader;

	std::unique_ptr<TextureClass[]> _textures;
	
	std::unique_ptr<FontShaderClass> _fontShader;
	std::unique_ptr<FontClass> _font;
	std::unique_ptr<TextClass[]> _textStrings;

	std::unique_ptr<LightClass[]> _lights;
	
	int _numLights;
	int _stringCount;

public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(InputClass&);

private:
	bool Render();
	bool UpdateMouseStrings(int, int, bool);
};