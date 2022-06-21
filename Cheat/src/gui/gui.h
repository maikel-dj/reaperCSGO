#pragma once
#include "../includes.h"

namespace gui
{
	//show menu?
	inline bool open = true;

	//is menu setup?
	inline bool setup = false;
	
	//winapi related
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = {};
	inline WNDPROC originalWindowProcess = nullptr;
	
	//dx stuff
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;

	bool SetupWindowClass(const wchar_t* windowClassName) noexcept;
	void DestroyWindowClass() noexcept;
	
	bool SetupWindow(const wchar_t* windowName) noexcept;
	void DestroyWindow() noexcept;

	bool SetupDirectX() noexcept;
	void DestroyDirectX() noexcept;
	
	//setup device
	void Setup() noexcept;	//setup everything
	
	void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void DestroyMenu() noexcept;

	void Render() noexcept;
}

