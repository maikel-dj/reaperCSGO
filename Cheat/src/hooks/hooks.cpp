#include "hooks.h"
#include "../includes.h"

//interfaces
static void* g_Client = nullptr;
static void* g_ClientMode = nullptr;

//get interface function
//used to get address in  memory of g_Client to use that for g_ClientMode which is needed to hook CreateMove
template <typename T>
static T* GetInterface(const char* name, const char* library)
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

void hooks::Destroy() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
	static const auto returnAddress = _ReturnAddress();

	const auto result = EndSceneOriginal(device, device);
	
	// stop endscene getting called twice
	if (_ReturnAddress() == returnAddress)
		return result;

	if (!gui::setup) {
		gui::SetupMenu(device);
		gui::ApplyCustomStyle();
		gui::LoadFonts();
	}

	if (gui::open)
		gui::Render(device);

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

//function storing
using CreateMove = bool(__thiscall*)(void*, float, UserCmd*);
static CreateMove CreateMoveOriginal = nullptr;

bool __stdcall CreateMoveHook(float frameTime, UserCmd* cmd)
{
	//need to call original function in a hook
	const bool result = CreateMoveOriginal(g_ClientMode, frameTime, cmd);

	if (!cmd || !cmd->commandNumber)
	{
		return result;
	}

	//experimental retcheck implementation
	static const auto returnAddress = _ReturnAddress();
	if (_ReturnAddress() == returnAddress)
		//patch function return with a nullptr
		
	
	//do your magic in here ! 

	//do your magic in here ! ^

	return false;
}

//maybe do surfacepaint hook for ESP?

void hooks::Setup()
{
	g_Client = GetInterface<void>("VClient018", "client.dll");
	g_ClientMode = **reinterpret_cast<void***>((*reinterpret_cast<unsigned int**>(g_Client))[10] + 5);
	
	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize minhook");

	if (MH_CreateHook(
		VirtualFunction(gui::device, 42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	)) throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(
		VirtualFunction(gui::device, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	)) throw std::runtime_error("Unable to hook Reset()");

	//createMove
	if (MH_CreateHook(
		(*static_cast<void***>(g_ClientMode))[24],
		&CreateMoveHook,
		reinterpret_cast<void**>(&CreateMoveOriginal)
	)) throw std::runtime_error("Unable to hook CreateMove()");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Unable to enable hooks");

	gui::DestroyDirectX();
}