#pragma once

#include <string>
#include <vector>
#include "IListConnector.hpp"

namespace EasyH {

	template <typename T>
	class JsonListConnector : IListConnector<T> {
	public:
		virtual std::string GetDefaultPath() override;
		virtual std::string GetExtensionName() override;

		virtual std::vector<T> ReadData(std::string path) override;
		virtual void Save(std::vector<T> data, std::string path) override;

	};

	template<typename T>
	inline std::vector<T> JsonListConnector<T>::ReadData(std::string path)
	{
		return std::map<K, V>();
	}
}