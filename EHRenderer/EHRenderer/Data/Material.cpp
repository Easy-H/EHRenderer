#include "Material.hpp"
#include "ShaderBase.hpp"
#include "MaterialUtils.hpp"

#include <fstream>

bool Material::Initialize(RenderEngineBase* engine, const std::string& addr)
{
	std::ifstream f(addr);

	nlohmann::json data = nlohmann::json::parse(f);

	if (!data.contains("shader")){
		return false;
	}

	_shader = engine->GetShader(data["shader"].get<std::string>());

	if (data.contains("textures"))
	{
		_textureCnt = data["textures"].size();
		_textures = std::make_unique<int[]>(_textureCnt);

		for (int i = 0; i < _textureCnt; i++)
		{
			_textures[i] = engine->RegisterTexture(
				data["textures"][i].get<std::string>());
		}
		
	}

	_slotInforSize = _shader->GetSlotCnt();
	_slotInfor = std::make_unique<SlotInfor[]>(_slotInforSize);

	if (data.contains("property")) {

		int propertyCnt = data["property"].size();

		for (int i = 0; i < propertyCnt; i++) {
			nlohmann::json p = data["property"][i];

			int slotIdx;
			size_t size;
			_shader->GetSlot(p["name"].get<std::string>(), slotIdx, size);

			if (slotIdx < 0) continue;
			
			nlohmann::json value = p["value"];
			_slotInfor[slotIdx].size = size;
			_slotInfor[slotIdx].isReset = false;
			_slotInfor[slotIdx].value = std::make_unique<char[]>(_slotInfor[slotIdx].size);

			char* ptr = &(_slotInfor[slotIdx].value[0]);

			for (int j = 0; j < value.size(); j++) {

				size_t valueSize;
				std::unique_ptr<char[]> buffer = GetTypeValue(value[j], valueSize);

				memcpy(ptr, &buffer[0], valueSize);
				ptr += valueSize;
			}
			
		}
		
	}

	return true;


}

bool Material::Render(RenderEngineBase* engine, int indexCount, const Transform* position)
{
	for (int i = 0; i < _slotInforSize; i++)
	{
		if (_slotInfor[i].isReset) {
			_shader->ResetSlot(i, _slotInfor[i].size);
			continue;
		}
		_shader->SetSlot(i, 0, &(_slotInfor[i].value[0]), _slotInfor[i].size);
	}

	for (int i = 0; i < _textureCnt; i++)
	{
		_shader->BindTexture(i, _textures[i]);
	}
	
	return _shader->Render(indexCount, position);

}