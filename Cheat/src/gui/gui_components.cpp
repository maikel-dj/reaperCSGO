#include "gui_components.h"
#include "gui.h"
#include "gui_consts.h"

#include <d3dx9tex.h>

// Every single component that is needed for the menu will be put here
// 

void gui_components::CreateMainMenu(IDirect3DDevice9* device) {
	ImGui::SetNextWindowSize(MAIN_MENU_SIZE);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(-1.0f, -1.0f)); // get rid of the top and left border of sidebar
	ImGui::Begin("REAPER | CS:GO", &gui_components::state.open, MAIN_MENU_FLAGS);
	ImGui::PopStyleVar();

	gui_components::CreateSidebar(device);

	ImGui::End();
}

// Functions related to drawing the main menu sidebar
void gui_components::DrawSidebarHorizontalLine(float lineThickness) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (!window) return;

	ImVec2 wndPos = ImGui::GetWindowPos();
	ImVec2 lastElementPos = window->DC.CursorPos - wndPos; // last element's bottom right pos coord
	ImGuiStyle& style = ImGui::GetStyle();

	window->DrawList->AddLine(
		ImVec2(wndPos.x, wndPos.y + lastElementPos.y + style.ItemSpacing.y),
		ImVec2(wndPos.x + SIDEBAR_WIDTH, wndPos.y + lastElementPos.y + style.ItemSpacing.y),
		ImGui::GetColorU32(style.Colors[ImGuiCol_Border]),
		lineThickness
	);

	window->DC.CursorPos.y += lineThickness+2*style.ItemSpacing.y; // make some space for the buttons under this line
}

void gui_components::DrawSidebarLogo(IDirect3DDevice9* device) {
	// TODO: put img in memory in the future
	if (!gui_components::state.logoTexture) {
		HRESULT res = D3DXCreateTextureFromFile(device, "C:\\Users\\madis\\source\\repos\\reaperCSGO\\Cheat\\images\\logo.png", &gui_components::state.logoTexture);
		if (res != S_OK) return;

		D3DSURFACE_DESC levelDesc;
		gui_components::state.logoTexture->GetLevelDesc(0, &levelDesc);
	}

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (!window) return;

	// add some padding for the logo in case it's stuck on the top left corner
	if(ImGui::GetStyle().WindowPadding.x != 0.0f) window->DC.CursorPos.x += 8.0f;
	if(ImGui::GetStyle().WindowPadding.y != 0.0f) window->DC.CursorPos.y += 8.0f;

	ImGui::Image((void*)gui_components::state.logoTexture, ImVec2(50.0f, 50.0f));
}

void gui_components::CreateSidebar(IDirect3DDevice9* device) {
	ImGui::BeginChild("Sidenav", ImVec2(SIDEBAR_WIDTH, MAIN_MENU_SIZE.y + 2), true);

	// header that has reaper logo and says welcome back, user
	gui_components::DrawSidebarLogo(device);
	gui_components::DrawSidebarHorizontalLine(5.0f);
	ImGui::Button("bbc");

	// buttons for navigation

	ImGui::EndChild();
}