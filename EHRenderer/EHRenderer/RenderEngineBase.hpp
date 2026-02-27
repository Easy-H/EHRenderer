#ifndef _RENDERENGINEBASE_H_
#define _RENDERENGINEBASE_H_

#include <string>

class ShaderBaseBase;
class Transform;

class RenderEngineBase {
public:
	virtual bool Initialize(int screenWidth, int screenHeight, bool fullscreen) = 0;

	virtual void RegisterRenderUnit(const std::string& modelAddr,
		const std::string& materialAddr, const Transform* transform) = 0;

	virtual int RegisterModel(const std::string& addr) = 0;
	virtual int RegisterTexture(const std::string& addr) = 0;
	virtual int RegisterFont(const std::string& addr) = 0;

	virtual ShaderBaseBase* GetShader(const std::string& name) = 0;

	virtual bool Render() = 0;
};

#endif