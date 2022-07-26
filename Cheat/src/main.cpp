#define WIN32_LEAN_AND_MEAN

#include "includes.h"
#include "netvars/netvars.h"
#include "hooks/hooks.h"
#include "interfaces/interfaces.h"
#include "memory/memory.h"
	

// setup function
void Setup(const HMODULE instance)
{

	try
	{
		//call main imgui and hooking setup functions
		gui::Setup();
		interfaces::Setup();
		netvars::SetupNetvars();
		memory::SetupValues();
		hooks::Setup();
	}
	catch (const std::exception& error)
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(
			0,
			error.what(),
			"REAPER exception",
			MB_OK | MB_ICONEXCLAMATION
		);

		goto UNLOAD;
	}

	//ejection (temporairly for testing in dev build)
	while (!GetAsyncKeyState(VK_NUMPAD9))
		Sleep(200);

UNLOAD:
	hooks::Destroy();
	gui::Destroy();

	FreeLibraryAndExitThread(instance, 0);
}

// entry point
BOOL WINAPI DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);

		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup),
			instance,
			0,
			nullptr
		);

		if (thread)
			CloseHandle(thread);
	}

	return TRUE;
}

