#pragma once
#include "BlurShaderClass.hpp"

class D3DClass;
class CameraClass;
class RenderTextureClass;
class TextureShaderClass;
class OrthoWindowClass;

class BlurClass {
public:
	BlurClass();
	BlurClass(const BlurClass&);
	~BlurClass();

	bool Initialize(D3DClass*, int, int, float, float, int, int);
	void Shutdown();

	bool BlurTexture(D3DClass*, CameraClass*, RenderTextureClass*, TextureShaderClass*, BlurShaderClass*);

private:
	RenderTextureClass* _downSampleTexure1;
	RenderTextureClass* _downSampleTexure2;
	OrthoWindowClass* _downSampleWindow;
	OrthoWindowClass* _upSampleWindow;
	int _downSampleWidth, _downSampleHeight;
};