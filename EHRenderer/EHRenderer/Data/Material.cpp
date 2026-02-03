#include "Material.hpp"
#include "../../External/json.hpp"
#include "ShaderBase.hpp"

#include <fstream>

bool Material::Initialize(RenderEngineBase* engine, char* addr)
{
	std::ifstream f(addr);

	nlohmann::json data = nlohmann::json::parse(f);

	if (!data.contains("shader")){
		return false;
	}

	_shader = engine->GetShader(data["shader"].get<std::string>().c_str());

	if (data.contains("textures"))
	{
		_textureCnt = data["textures"].size();

		for (int i = 0; i < _textureCnt; i++)
		{
			_textures[i] = engine->RegisterTexture(
				data["textures"][i].get<std::string>().c_str());
		}
		
	}

	return true;


}

bool Material::Render(RenderEngineBase* engine, int indexCount)
{
	for (int i = 0; i < _textureCnt; i++)
	{
		_shader->BindTexture(i, _textures[i]);
	}
	
	return _shader->Render(indexCount);

}
