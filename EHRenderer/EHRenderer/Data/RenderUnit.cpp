#include "RenderUnit.hpp"

bool RenderUnit::Initialize(RenderEngineBase* engine, char* modelAddr, char* materialAddr)
{
	_modelId = engine->RegisterModel(modelAddr);
	_materialId = engine->RegisterTexture(materialAddr);

	return true;
}

bool RenderUnit::Render() {
	return true;
}