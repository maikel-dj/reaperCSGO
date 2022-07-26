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
bool __stdcall hooks::CreateMoveHook(float frameTime, UserCmd* cmd)
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
	
	//force crosshair
	static auto cross = interfaces::cvar->FindVar("weapon_debug_spread_show");
	cross->SetValue(3);

	//do your magic in here ! ^

	return false;
}

void* __stdcall hooks::AllocKeyValuesMemoryHook(const int32_t size) noexcept {
	// UPDATED VERSION:: check if we are returning somewhere inside a designated function to check the return address (pRetCheck <= addr && addr < pRetCheck+200 bytes)
	if (const uint32_t address = reinterpret_cast<uint32_t>(_ReturnAddress());
		(reinterpret_cast<uint32_t>(memory::allocKeyValuesEngine) <= address && address < reinterpret_cast<uint32_t>(memory::allocKeyValuesEngine) + 200) ||
		(reinterpret_cast<uint32_t>(memory::allocKeyValuesClient) <= address && address < reinterpret_cast<uint32_t>(memory::allocKeyValuesClient) + 200)
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

void __stdcall hooks::PaintTraverseHook(uintptr_t vguiPanel, bool forceRepaint, bool allowForce) noexcept {
	CEntity* localPlayer = (CEntity*)interfaces::entityList->GetClientEntity(1);

	// check if we have the right panel
	if (vguiPanel == interfaces::engineVGui->GetPanel(PANEL_TOOLS)) {
		if (localPlayer && interfaces::engine->IsInGame()) {
			for (int i = 1; i <= 64; i++) {
 				CEntity* player = (CEntity*)interfaces::entityList->GetClientEntity(i);

				if (!player) continue;
				if (player->IsDormant() || !player->IsAlive()) continue;
				if (localPlayer->GetTeam() == player->GetTeam()) continue;
				// don't draw esp on people we are spectating
				if (!localPlayer->IsAlive() && localPlayer->GetObserverTarget() == player) continue;


				CMatrix3x4 bones[128];
				if (!player->SetupBones(bones, 128, 0x7FF00, interfaces::globals->currentTime)) continue;

				// screen position above head
				CVector top;
				if (interfaces::debugOverlay->ScreenPosition(bones[0].Origin() + CVector{ 0.0f, 0.0f, 11.0f }, top)) continue;

				// screen position below feet
				CVector bottom;
				if (interfaces::debugOverlay->ScreenPosition(player->GetAbsOrigin() - CVector{ 0.0f, 0.0f, 9.0f }, bottom)) continue;

				float boxHeight = bottom.y - top.y;
				float boxWidth = boxHeight * 0.1f;

				int left = static_cast<int>(top.x - boxWidth);
				int right = static_cast<int>(top.x + boxHeight);

				interfaces::surface->DrawSetColor(255, 255, 255, 255);
				interfaces::surface->DrawOutlinedRect(left, top.y, right, bottom.y);
			}
		}
	}
	
	PaintTraverseOriginal(interfaces::panel, vguiPanel, forceRepaint, allowForce);
}

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
	)) throw std::runtime_error("Unable to hook AllocKeyValues()");

	// VGUI lock cursor hook to fix inability to move menu in game
	if (MH_CreateHook(
		VirtualFunction(interfaces::surface, 67),
		&LockCursorHook,
		reinterpret_cast<void**>(&LockCursorOriginal)
	)) throw std::runtime_error("Unable to hook LockCursor()");

	// PaintTraverse
	if (MH_CreateHook(
		VirtualFunction(interfaces::panel, 41),
		&PaintTraverseHook,
		reinterpret_cast<void**>(&PaintTraverseOriginal)
	)) throw std::runtime_error("Unable to hook PaintTraverse()");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Unable to enable hooks");

	gui::DestroyDirectX();
}