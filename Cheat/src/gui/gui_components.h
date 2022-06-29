#pragma once
#include "../includes.h"

struct MenuState {
	bool open;

	// textures
	PDIRECT3DTEXTURE9 logoTexture;
};

namespace gui_components {
	inline MenuState state = { 0 };

	void DrawSidebarHorizontalLine(float lineThickness);
	void DrawSidebarLogo(IDirect3DDevice9* device);
	void CreateSidebar(IDirect3DDevice9* device);

	void CreateMainMenu(IDirect3DDevice9* device);
}