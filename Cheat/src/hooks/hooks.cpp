#include "hooks.h"
#include "../includes.h"
#include "../interfaces/interfaces.h"
#include "../memory/memory.h"
#include "../client-classes/CEntity.h"

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

	//retarded check that will break everything if we dont use it
	if (cmd->commandNumber == 0)
		return false;

	//need to call original function in a hook
	const bool result = CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);

	if (!cmd || !cmd->commandNumber)
	{
		return result;
	}
	
	//do your magic in here ! 

	// Radar hack: on by default
	for (int i = 1; i <= 64; i++) {
		CEntity* entity = (CEntity*)interfaces::entityList->GetClientEntity(i);
		if (!entity) continue;

		entity->Spotted() = true;
	}

	//do your magic in here ! ^

	return false;
}

void* __stdcall hooks::AllocKeyValuesMemoryHook(const std::int32_t size) noexcept {
	// UPDATED VERSION:: check if we are returning somewhere inside a designated function to check the return address (pRetCheck <= addr && addr < pRetCheck+200 bytes)
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		(reinterpret_cast<std::uint32_t>(memory::allocKeyValuesEngine) <= address && address < reinterpret_cast<std::uint32_t>(memory::allocKeyValuesEngine) + 200) ||
		(reinterpret_cast<std::uint32_t>(memory::allocKeyValuesClient) <= address && address < reinterpret_cast<std::uint32_t>(memory::allocKeyValuesClient) + 200)
	) return nullptr;

	// return original
	return AllocKeyValuesMemoryOriginal(interfaces::keyValuesSystem, size);
}

void __stdcall hooks::LockCursorHook() {
	if (gui::open) {
		using UnlockCursorFn = void(__thiscall*)(void*);
		UnlockCursorFn unlockCursor = (UnlockCursorFn)VirtualFunction(interfaces::surface, 66);

		unlockCursor(interfaces::surface);
	}
	else {
		LockCursorOriginal(interfaces::surface);
	}
}

//maybe do surfacepaint hook for ESP?

void hooks::Setup()
{
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
		(*static_cast<void***>(interfaces::clientMode))[24],
		&CreateMoveHook,
		reinterpret_cast<void**>(&CreateMoveOriginal)
	)) throw std::runtime_error("Unable to hook CreateMove()");

	//retcheck bypass hook
	if (MH_CreateHook(
		VirtualFunction(interfaces::keyValuesSystem, 1),
		&AllocKeyValuesMemoryHook,
		reinterpret_cast<void**>(&AllocKeyValuesMemoryOriginal)
	)) throw std::runtime_error("Unable to hook CreateMove()");

	// VGUI lock cursor hook to fix inability to move menu in game
	if (MH_CreateHook(
		VirtualFunction(interfaces::surface, 67),
		&LockCursorHook,
		reinterpret_cast<void**>(&LockCursorOriginal)
	)) throw std::runtime_error("Unable to hook CreateMove()");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Unable to enable hooks");

	gui::DestroyDirectX();
}