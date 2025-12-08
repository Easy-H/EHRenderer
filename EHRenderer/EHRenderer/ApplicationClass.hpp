#include <windows.h>
#include <memory>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class D3DClass;
class CameraClass;
class SimpleModelClass;
class ModelClass;
class ColorShaderClass;
class TextureShaderClass;
class LightShaderClass;
class LightClass;

class ApplicationClass {
private:
	std::unique_ptr<D3DClass> _direct3D;
	std::unique_ptr<CameraClass> _camera;
	std::unique_ptr<ModelClass> _model;
	std::unique_ptr<SimpleModelClass> _simpleModel;
	std::unique_ptr<ColorShaderClass> _colorShader;
	std::unique_ptr<TextureShaderClass> _textureShader;
	std::unique_ptr<LightShaderClass> _lightShader;
	std::unique_ptr<LightClass> _light;

public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render(float);
};