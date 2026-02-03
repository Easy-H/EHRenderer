#pragma once

#include <string>
#include <map>

namespace EasyH {

	template <typename K, typename V>
	class IDictionaryConnector {
	public:
		virtual std::string GetDefaultPath() = 0;
		virtual std::string GetExtensionName() = 0;

		virtual std::map<K, V> ReadData(std::string path) = 0;
		virtual void Save(std::map<K, V> data, std::string path) = 0;

	};
}