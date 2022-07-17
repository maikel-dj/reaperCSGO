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
	client = GetInterface<void*>("VClient018", "client.dll");
	clientMode = **reinterpret_cast<void***>((*reinterpret_cast<unsigned int**>(client))[10] + 5);

	HMODULE hMod = GetModuleHandle("vstdlib.dll");
	if(hMod)
		keyValuesSystem = reinterpret_cast<void* (__cdecl*)()>(GetProcAddress(hMod, "KeyValuesSystem"))();
}