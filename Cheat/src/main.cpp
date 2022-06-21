#define WIN32_LEAN_AND_MEAN
#include "includes.h"
#include "hooks/hooks.h"
	
//setup function
void Setup(const HMODULE instance)
{
	//create thread
	try
	{
		gui::Setup();
		hooks::Setup();
	}

	catch (const std::exception& error)
	{
		MessageBeep(MB_ICONERROR);

		//maybe if it crashes change project to multichar
		MessageBox(0, (wchar_t*)error.what(), L"reaper exception", MB_OK | MB_ICONEXCLAMATION);

		goto UNLOAD;
	}

	while (!GetAsyncKeyState(VK_NUMPAD9))
	{
		Sleep(200);
	}

UNLOAD:
	hooks::Destroy();
	gui::DestroyMenu();

	FreeLibraryAndExitThread(instance, 0);

}

//entry point
BOOL WINAPI MainEntry(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);

		const auto thread = CreateThread(
			nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup), instance, 0, nullptr
		);

		if (thread)
			CloseHandle(thread);
	}
	return TRUE;
}

