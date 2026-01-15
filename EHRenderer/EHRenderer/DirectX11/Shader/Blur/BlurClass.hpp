#pragma once

#include "BlurShaderClass.hpp"
#include <memory>

class D3DClass;
class CameraClass;
class RenderTextureClass;
class TextureShaderClass;
class OrthoWindowClass;

using namespace std;

class BlurClass {
public:
	BlurClass();
	BlurClass(const BlurClass&);
	~BlurClass();

	bool Initialize(D3DClass*, int, int, float, float, int, int);
	void Shutdown();

	bool BlurTexture(D3DClass*, CameraClass*, RenderTextureClass*, TextureShaderClass*, BlurShaderClass*);

private:
	unique_ptr<RenderTextureClass> _downSampleTexture1;
	unique_ptr<RenderTextureClass> _downSampleTexture2;

	unique_ptr<OrthoWindowClass> _downSampleWindow;
	unique_ptr<OrthoWindowClass> _upSampleWindow;

	int _downSampleWidth, _downSampleHeight;
};