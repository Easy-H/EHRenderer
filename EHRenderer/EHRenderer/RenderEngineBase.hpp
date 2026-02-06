#ifndef _RENDERENGINEBASE_H_
#define _RENDERENGINEBASE_H_

class ShaderBaseBase;
class Transform;

class RenderEngineBase {
public:
	virtual bool Initialize(int screenWidth, int screenHeight, bool fullscreen) = 0;

	virtual void RegisterRenderUnit(const char* modelAddr,
		const char* materialAddr, const Transform* transform) = 0;

	virtual int RegisterModel(const char* addr) = 0;
	virtual int RegisterTexture(const char* addr) = 0;
	virtual int RegisterFont(const char* addr) = 0;

	virtual ShaderBaseBase* GetShader(const char* name) = 0;

	virtual bool Render() = 0;
};

#endif