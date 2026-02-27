#ifndef _MATERIALUTILS_H_
#define _MATERIALUTILS_H_

#include <string>
#include <memory>
#include "../../External/json.hpp"

struct float4 {
	float x;
	float y;
	float z;
	float w;
};
struct float2 {
	float x;
	float y;
};
struct float3 {
	float x;
	float y;
	float z;
};

static std::unique_ptr<char[]> GetTypeValue(const nlohmann::json& j, size_t& size)
{
	std::string type = j["type"].get<std::string>();

	if (type.compare("float4") == 0) {
		std::unique_ptr<char[]> ret = std::make_unique<char[]>(16);
		char* ptr = &ret[0];
		float4 value{
			j["value"][0].get<float>(),
			j["value"][1].get<float>(),
			j["value"][2].get<float>(),
			j["value"][3].get<float>()
		};
		memcpy(ptr, &value, 16);
		size = 16;
		return std::move(ret);
	}
	if (type.compare("float3") == 0) {
		std::unique_ptr<char[]> ret = std::make_unique<char[]>(16);
		char* ptr = &ret[0];
		float3 value{
			j["value"][0].get<float>(),
			j["value"][1].get<float>(),
			j["value"][2].get<float>()
		};
		memcpy(ptr, &value, 12);
		size = 12;
		return std::move(ret);
	}
	if (type.compare("float2") == 0) {
		std::unique_ptr<char[]> ret = std::make_unique<char[]>(16);
		char* ptr = &ret[0];
		float2 value{
			j["value"][0].get<float>(),
			j["value"][1].get<float>(),
		};
		memcpy(ptr, &value, 8);
		size = 8;
		return std::move(ret);

	}
	if (type.compare("float") == 0) {

		std::unique_ptr<char[]> ret = std::make_unique<char[]>(4);
		float value = j["value"].get<float>();
		memcpy(&ret[0], &value, 4);
		size = 4;
		return std::move(ret);

	}

	return nullptr;
}

#endif