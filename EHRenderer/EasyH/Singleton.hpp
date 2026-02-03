#pragma once

#include <memory>

namespace EasyH {
	template <typename T>
	class Singleton {
	private:
		static std::unique_ptr<T> _instance;
	public:
		Singleton() {}
		virtual ~Singleton() {}
		static T& GetInstance() {
			if (_instance == nullptr) {
				_instance = std::make_unique<T>();
			}
			return *_instance;
		}
		Singleton(Singleton const&) = delete;
		Singleton& operator=(Singleton const&) = delete;

	};

}

template <typename T>
std::unique_ptr<T> EasyH::Singleton<T>::_instance = nullptr;