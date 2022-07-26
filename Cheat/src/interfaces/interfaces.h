#pragma once
#include "../includes.h"

#include "../../ext/valve-sdk/IBaseClientDLL.h"
#include "../../ext/valve-sdk/IClientEntityList.h"
#include "../../ext/valve-sdk/IInputSystem.h"
#include "../../ext/valve-sdk/ICvar.h"
#include "../../ext/valve-sdk/IEngineVGui.h"
#include "../../ext/valve-sdk/IVDebugOverlay.h"

#include "../client-classes/convar.h"
#include "../client-classes/IVEngineClient.h"
#include "../client-classes/GlobalVarsBase.h"
#include "../client-classes/ISurface.h"

namespace interfaces {
	inline IBaseClientDLL* client = nullptr;
	inline IClientEntityList* entityList = nullptr;
	inline IInputSystem* inputSystem = nullptr;
	inline ICvar* cvar = nullptr;
	inline ISurface* surface = nullptr;
	inline void* clientMode = nullptr;
	inline void* keyValuesSystem = nullptr;
	inline void* panel = nullptr;
	inline IEngineVGui* engineVGui = nullptr;
	inline IVEngineClient* engine = nullptr;
	inline GlobalVarsBase* globals = nullptr;
	inline IVDebugOverlay* debugOverlay = nullptr;

	template <typename T>
	static T* GetInterface(const char* name, const char* library);
	void Setup();
}
