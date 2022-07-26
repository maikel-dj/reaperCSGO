#include "interfaces.h"

template <typename T>
static T* interfaces::GetInterface(const char* name, const char* library)
{
	const HINSTANCE handle = GetModuleHandle(library);

	if (!handle)
	{
		//optional error checking here but for now unnecesary
		return nullptr;
	}

	using Fn = T * (*)(const char*, int*);
	const Fn CreateInterface = reinterpret_cast<Fn>(GetProcAddress(handle, "CreateInterface"));

	return CreateInterface(name, nullptr);
}

void interfaces::Setup() {
	client = GetInterface<IBaseClientDLL>("VClient018", "client.dll");
	entityList = GetInterface<IClientEntityList>("VClientEntityList003", "client.dll");
	surface = GetInterface<ISurface>("VGUI_Surface031", "vguimatsurface.dll");
	inputSystem = GetInterface<IInputSystem>("InputSystemVersion001", "inputsystem.dll");
	cvar = GetInterface<ICvar>("VEngineCvar007", "vstdlib.dll");
	clientMode = **reinterpret_cast<void***>((*reinterpret_cast<unsigned int**>(client))[10] + 5);
	panel = GetInterface<void>("VGUI_Panel009", "vgui2.dll");
	engineVGui = GetInterface<IEngineVGui>("VEngineVGui001", "engine.dll");
	engine = GetInterface<IVEngineClient>("VEngineClient014", "engine.dll");
	debugOverlay = GetInterface<IVDebugOverlay>("VDebugOverlay004", "engine.dll");

	if (!(globals = **reinterpret_cast<GlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(client))[11] + 10)))
	{
		throw std::runtime_error("Unable to get GlobalVars pointer.");
	}

	HMODULE hMod = GetModuleHandle("vstdlib.dll");
	if(hMod)
		keyValuesSystem = reinterpret_cast<void* (__cdecl*)()>(GetProcAddress(hMod, "KeyValuesSystem"))();
}