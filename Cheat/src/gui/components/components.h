#pragma once

#include "../../includes.h"

struct MenuState {
	bool open;
	int currentActiveTab = 0;

	// textures
	PDIRECT3DTEXTURE9 logoTexture;
	PDIRECT3DTEXTURE9 avatarTexture;
};

namespace gui_components {
	inline MenuState state = {0};

	void CreateMainMenu(IDirect3DDevice9* device);
	void DrawHorizontalSeparator(float thickness);
	void CreateCustomTitlebar(IDirect3DDevice9* device);
	void CreateTabBar(IDirect3DDevice9* device);
}