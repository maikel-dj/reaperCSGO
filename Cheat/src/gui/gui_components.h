#pragma once
#include "../includes.h"

struct MenuState {
	bool open;

	// textures
	PDIRECT3DTEXTURE9 logoTexture;
	PDIRECT3DTEXTURE9 avatarTexture;
};

namespace gui_components {
	inline MenuState state = { 0 };

	void CreateTitlebar(IDirect3DDevice9* device);
	void DrawTitlebarLogo(IDirect3DDevice9* device);

	void DrawUserAvatar(IDirect3DDevice9* device);
	void DrawSidebarHorizontalLine(float lineThickness);
	void CreateSidebar(IDirect3DDevice9* device);

	void CreateMainMenu(IDirect3DDevice9* device);
}