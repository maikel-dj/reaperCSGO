#pragma once

#include <d3dx9tex.h>

void LoadTextureFromFile(IDirect3DDevice9* device, const char* filepath, PDIRECT3DTEXTURE9& tex) {
	HRESULT res = D3DXCreateTextureFromFile(device, filepath, &tex);
	if (res != S_OK) return;

	D3DSURFACE_DESC levelDesc;
	tex->GetLevelDesc(0, &levelDesc);
}