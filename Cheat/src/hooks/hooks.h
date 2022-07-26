#pragma once

#include "../includes.h"
#include "../gui/gui.h"

namespace hooks
{
	void Setup();
	void Destroy() noexcept;

	constexpr void* VirtualFunction(void* thisptr, size_t index) noexcept
	{
		return (*static_cast<void***>(thisptr))[index];
	}

	using EndSceneFn = long(__thiscall*)(void*, IDirect3DDevice9*) noexcept;
	inline EndSceneFn EndSceneOriginal = nullptr;
	long __stdcall EndScene(IDirect3DDevice9* device) noexcept;

	using ResetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline ResetFn ResetOriginal = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;

	using CreateMoveFn = bool(__thiscall*)(void*, float, UserCmd*);
	inline CreateMoveFn CreateMoveOriginal = nullptr;
	bool __stdcall CreateMoveHook(float frameTime, UserCmd* cmd);

	using AllocKeyValuesMemoryFn = void* (__thiscall*)(void*, const int32_t) noexcept;
	inline AllocKeyValuesMemoryFn AllocKeyValuesMemoryOriginal = nullptr;
	void* __stdcall AllocKeyValuesMemoryHook(const int32_t size) noexcept;

	using LockCursorFn = void (__thiscall*)(void*);
	inline LockCursorFn LockCursorOriginal = nullptr;
	void __stdcall LockCursorHook();
	
	using PaintTraverseFn = void(__thiscall*)(void*, uintptr_t, bool, bool);
	inline PaintTraverseFn PaintTraverseOriginal = nullptr;
	void __stdcall PaintTraverseHook(uintptr_t vguiPanel, bool forceRepaint, bool allowForce) noexcept;
}
