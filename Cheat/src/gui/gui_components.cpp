#include "gui_components.h"
#include "gui_consts.h"
#include "gui.h"

#include <d3dx9tex.h>

// Every single component that is needed for the menu will be put here
// 

void gui_components::CreateMainMenu(IDirect3DDevice9* device) {
	ImGui::SetNextWindowSize(MAIN_MENU_SIZE);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(-1.0f, -1.0f)); // get rid of the top and left border of sidebar
	ImGui::Begin("REAPER | CS:GO", &gui_components::state.open, MAIN_MENU_FLAGS);
	ImGui::PopStyleVar();

	gui_components::CreateTitlebar(device);
	gui_components::CreateSidebar(device);

	ImGui::End();
}

// Functions related to drawing our custom title bar
void gui_components::CreateTitlebar(IDirect3DDevice9* device) {
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, -1.0f)); // get rid of the border at the top
	ImGui::BeginChild("Titlebar", ImVec2(windowSize.x+2, TITLEBAR_HEIGHT), true);
	ImGui::PopStyleVar();

	// calculate text & logo width to center them
	ImGui::PushFont(gui::logoFont);
	float windowWidth = ImGui::GetWindowSize().x;
	float titleContentWidth = TITLEBAR_LOGO_SIZE.x + ImGui::CalcTextSize("REAPER | CS:GO").x;
	ImGui::SetCursorPosX((windowWidth - titleContentWidth) * 0.5f);
	gui_components::DrawTitlebarLogo(device);
	ImGui::SameLine();
	ImGui::Text("REAPER | CS:GO");

	ImGui::PopFont();
	ImGui::EndChild();
	//ImGui::PopStyleColor();
}

void gui_components::DrawTitlebarLogo(IDirect3DDevice9* device) {
	// TODO: put img in memory in the future
	if (!gui_components::state.logoTexture) {

		// autopilot recommended me this smile face :)
		// application wont crash if the texture is not found, but better practice to include it in the project next time and not use whole path
		HRESULT res = D3DXCreateTextureFromFile(device, "C:\\Users\\madis\\source\\repos\\reaperCSGO\\Cheat\\images\\logo.png", &gui_components::state.logoTexture);
		if (res != S_OK) return;

		D3DSURFACE_DESC levelDesc;
		gui_components::state.logoTexture->GetLevelDesc(0, &levelDesc);
	}

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (!window) return;

	// add some padding for the logo in case it's stuck to the top left corner
	if (ImGui::GetStyle().WindowPadding.x == 0.0f) window->DC.CursorPos.x += 8.0f;
	if (ImGui::GetStyle().WindowPadding.y == 0.0f) window->DC.CursorPos.y += 8.0f;
	ImGui::Image((void*)gui_components::state.logoTexture, TITLEBAR_LOGO_SIZE);
}

// Functions related to drawing the main menu sidebar
void gui_components::DrawUserAvatarAndData(IDirect3DDevice9* device) {
	// USER AVATAR
	// TODO: put img in memory in the future
	if (!gui_components::state.avatarTexture) {
		
		// application wont crash if the texture is not found, but better practice to include it in the project next time and not use whole path
		HRESULT res = D3DXCreateTextureFromFile(device, "C:\\Users\\madis\\source\\repos\\reaperCSGO\\Cheat\\images\\circle_question_mark.png", &gui_components::state.avatarTexture);
		if (res != S_OK) return;

		D3DSURFACE_DESC levelDesc;
		gui_components::state.avatarTexture->GetLevelDesc(0, &levelDesc);
	}

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (!window) return;

	// add some padding for the logo in case it's stuck to the top left corner
	if (ImGui::GetStyle().WindowPadding.x == 0.0f) window->DC.CursorPos.x += 8.0f;
	if (ImGui::GetStyle().WindowPadding.y == 0.0f) window->DC.CursorPos.y += 8.0f;
	ImGui::Image((void*)gui_components::state.avatarTexture, AVATAR_SIZE);

	// USER DATA (name, sub end)
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, style.ItemSpacing.y));

	if (ImGui::GetStyle().WindowPadding.x == 0.0f) window->DC.CursorPos.x += 8.0f;
	ImGui::TextWithFont(gui::defaultFont20, "Welcome back, ");
	ImGui::SameLine(); 
	ImGui::TextWithFont(gui::defaultFontBold20, "Sparkly");

	if (ImGui::GetStyle().WindowPadding.x == 0.0f) window->DC.CursorPos.x += 8.0f;
	ImGui::TextWithFont(gui::defaultFont20, "Subscription ends in ");
	ImGui::SameLine();
	ImGui::TextWithFont(gui::defaultFontBold20, "19 days");

	ImGui::PopStyleVar();
}

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

int selectedBtn = 0;
void gui_components::CreateSidebar(IDirect3DDevice9* device) {
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::BeginChild("Sidenav", ImVec2(SIDEBAR_WIDTH, MAIN_MENU_SIZE.y + 2), true);

	// header that has reaper logo and says welcome back, user
	gui_components::DrawUserAvatarAndData(device);
	// line separator
	gui_components::DrawSidebarHorizontalLine(5.0f);
	// buttons for different sections of the cheat (legitbot, misc etc)
	ImGui::PushFont(gui::defaultFont24);
	if (ImGui::SidebarButtonC("Aimbot", 0, selectedBtn == 0)) selectedBtn = 0;
	if (ImGui::SidebarButtonC("Visuals", 0, selectedBtn == 1)) selectedBtn = 1;
	if (ImGui::SidebarButtonC("Misc", 0, selectedBtn == 2)) selectedBtn = 2;
	ImGui::Button("Compare");
	ImGui::PopFont();


	ImGui::EndChild();
}