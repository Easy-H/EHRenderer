#include <windows.h>
#include <memory>
#include <directxmath.h>

using namespace std;
using namespace DirectX;

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.f;

class InputClass;
class TimerClass;

class ApplicationClass {
private:
	
	int _numLights;
	int _stringCount;
	int _numModels;

	float _waterHeight;
	float _waterTranslation;

	int _screenWidth;
	int _screenHeight;

	XMMATRIX _baseViewMatrix;

	std::unique_ptr<TimerClass> _timer;

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
	bool RenderSceneToTexture(int);
	bool RenderGlowToTexture(int);
	bool RenderDepthToTexture();
	bool RenderBlackAndWhilteShadows(int targetIdx);
	bool RenderRefractionToTexture();
	bool RenderReflectionToTexture();

	bool TestIntersection(int, int);
	bool RaySphereIntersect(XMFLOAT3, XMFLOAT3, float);

	bool UpdateMouseStrings(int, int, bool);
	bool UpdateRenderCountString(int);
};
