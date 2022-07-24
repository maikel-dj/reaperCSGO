#pragma once
#include "../includes.h"

namespace gui
{
	// show menu?
	inline bool open = true;
	// is menu setup?
	inline bool setup = false;
	// font data
	inline ImFont* logoFont = nullptr;
	inline ImFont* defaultFontBold = nullptr;
	inline ImFont* defaultFont16 = nullptr;
	inline ImFont* defaultFont20 = nullptr;
	inline ImFont* defaultFont24 = nullptr;
	inline ImFont* defaultFontBold16 = nullptr;
	inline ImFont* defaultFontBold20 = nullptr;
	inline ImFont* defaultFontBold24 = nullptr;

	// winapi related
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = {};
	inline WNDPROC originalWindowProcess = nullptr;

	// dx stuff
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;

	bool SetupWindowClass(const char* windowClassName) noexcept;
	void DestroyWindowClass() noexcept;

	bool SetupWindow(const char* windowName) noexcept;
	void DestroyWindow() noexcept;

	bool SetupDirectX() noexcept;
	void DestroyDirectX() noexcept;

	// setup device
	void Setup();

	void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void Destroy() noexcept;

	// everything related to modifying or drawing the menue
	void Render(IDirect3DDevice9* device) noexcept;
	void ApplyCustomStyle() noexcept;
	void LoadFonts() noexcept;
}

