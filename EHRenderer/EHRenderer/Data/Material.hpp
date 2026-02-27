#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <memory>
#include <string>
#include <vector>
#include "../../External/json.hpp"
#include "../RenderEngineBase.hpp"

class ShaderBaseBase;
class Transform;

class Material {
	struct SlotInfor {
		bool isReset = true;
		size_t size = -1;
		std::unique_ptr<char[]> value;
	};
public:
	bool Initialize(RenderEngineBase* engine, const std::string& addr);
	bool Render(RenderEngineBase* engine, int indexCount, const Transform* position);

	//void SetFloat(std::string& name, float value);
	//void SetFloat4(std::string& name, std::vector<float> value);
private:

	ShaderBaseBase* _shader;
	std::unique_ptr<SlotInfor[]> _slotInfor;
	int _slotInforSize;
	std::unique_ptr<int[]> _textures;
	int _textureCnt;
public:
};

#endif