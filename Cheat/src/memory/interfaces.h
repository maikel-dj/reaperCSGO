#pragma once
#include "../includes.h"

namespace interfaces {
	inline void* client = nullptr;
	inline void* clientMode = nullptr;
	inline void* keyValuesSystem = nullptr;

	template <typename T>
	static T* GetInterface(const char* name, const char* library);
	void Setup();
}
