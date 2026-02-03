#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <memory>
#include "../RenderEngineBase.hpp"

class ShaderBaseBase;

class Material {
public:
	bool Initialize(RenderEngineBase* engine, char* addr);
	bool Render(RenderEngineBase* engine, int indexCount);
private:
	ShaderBaseBase* _shader;
	std::unique_ptr<int[]> _textures;
	int _textureCnt;
};

#endif