#include <string>
#include <map>
#include "JsonListConnector.hpp"

namespace EasyH {

	template<typename T>
	inline std::string JsonListConnector<T>::GetDefaultPath()
	{
		return std::string();
	}

	template<typename T>
	std::string JsonListConnector<T>::GetExtensionName()
	{
		return std::string();
	}

	template<typename T>
	void JsonListConnector<T>::Save(std::vector<T> data, std::string path)
	{

	}

}