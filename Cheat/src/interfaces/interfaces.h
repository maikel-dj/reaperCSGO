#pragma once
#include "../includes.h"

#include "../../ext/valve-sdk/IBaseClientDLL.h"
#include "../../ext/valve-sdk/IClientEntityList.h"
#include "../../ext/valve-sdk/IInputSystem.h"

namespace interfaces {
	inline IBaseClientDLL* client = nullptr;
	inline IClientEntityList* entityList = nullptr;
	inline IInputSystem* inputSystem = nullptr;
	inline void* surface = nullptr;
	inline void* clientMode = nullptr;
	inline void* keyValuesSystem = nullptr;

	template <typename T>
	static T* GetInterface(const char* name, const char* library);
	void Setup();
}
