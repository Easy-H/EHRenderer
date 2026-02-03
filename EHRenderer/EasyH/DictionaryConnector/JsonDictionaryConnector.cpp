#include <string>
#include <map>
#include "JsonDictionaryConnector.hpp"

namespace EasyH {

	template<typename K, typename V>
	inline std::string JsonDictionaryConnector<K, V>::GetDefaultPath()
	{
		return std::string();
	}

	template<typename K, typename V>
	std::string JsonDictionaryConnector<K, V>::GetExtensionName()
	{
		return std::string();
	}

	template<typename K, typename V>
	void JsonDictionaryConnector<K, V>::Save(std::map<K, V> data, std::string path)
	{

	}

}