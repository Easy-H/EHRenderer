#ifndef _RENDERUNIT_H_
#define _RENDERUNIT_H_

#include "../RenderEngineBase.hpp"

class RenderUnit {
public:
	bool Initialize(RenderEngineBase* engine, char* modelAddr, char* materialAddr);
	bool Render();
private:
	int _modelId;
	int _materialId;
};

#endif