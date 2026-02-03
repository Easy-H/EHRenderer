#pragma once

#include <string>
#include <vector>

namespace EasyH {

	template <typename T>
	class IListConnector {
	public:
		virtual std::string GetDefaultPath() = 0;
		virtual std::string GetExtensionName() = 0;

		virtual std::vector<T> ReadData(std::string path) = 0;
		virtual void Save(std::vector<T> data, std::string path) = 0;

	};
}