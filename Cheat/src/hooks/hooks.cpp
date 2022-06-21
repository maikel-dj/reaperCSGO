#include "hooks.h"
#include "../includes.h"


void hooks::Setup()
{
	if (MH_Initialize())
	{
		throw std::runtime_error("Unable to initialize holy minhook");
	}

	if (MH_CreateHook(
		VirtualFunction(gui::device, 42),
		&EndSceneHook,
		reinterpret_cast<void**>(&EndSceneOriginal)
	))
	{
		throw std::runtime_error("Unable to hook EndScene");
	}
	
	if (MH_CreateHook(
		VirtualFunction(gui::device, 43),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	))
	{
		throw std::runtime_error("Unable to hook Reset");
	}

	if (MH_EnableHook(MH_ALL_HOOKS))
	{
		throw std::runtime_error("Unable to enable all hooks");
	}

	gui::DestroyDirectX();
}

void hooks::Destroy() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall hooks::EndSceneHook(IDirect3DDevice9* device) noexcept
{
	static const auto returnAddress = _ReturnAddress();

	const auto result = EndSceneOriginal(device, device);

	//stop endscene getting called twice
	if (returnAddress == _ReturnAddress())
	{
		return result;
	}

	if (!gui::setup)
	{
		gui::SetupMenu(device);
	}

	if (gui::open)
	{
		gui::Render();
	}

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}