#pragma once

#include <string>
#include <map>
#include "IDictionaryConnector.hpp"

namespace EasyH {

	template <typename K, typename V>
	class JsonDictionaryConnector : IDictionaryConnector<K, V> {
	public:
		virtual std::string GetDefaultPath() override;
		virtual std::string GetExtensionName() override;

		virtual std::map<K, V> ReadData(std::string path) override;
		virtual void Save(std::map<K, V> data, std::string path) override;

	};

	template<typename K, typename V>
	inline std::map<K, V> JsonDictionaryConnector<K, V>::ReadData(std::string path)
	{
		return std::map<K, V>();
	}
}