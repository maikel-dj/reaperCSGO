#include "components.h"
#include "../gui_consts.h"
#include "../gui.h"
#include "../helpers/directx.h"
#include "../../globals.h"

void gui_components::CreateMainMenu(IDirect3DDevice9* device) {
	ImGui::SetNextWindowSize(MAIN_MENU_SIZE);

	ImGui::Begin("REAPER | CS:GO", &state.open, MAIN_MENU_FLAGS);

	gui_components::CreateCustomTitlebar(device); // it's easier to do it this way rather than to modify imgui::begin code
	gui_components::DrawHorizontalSeparator(7.0f);
	// empty area for now
	ImGui::BeginChild("AimbotTab", ImVec2(MAIN_MENU_SIZE.x, 550.0f));
	ImGui::EndChild();
	gui_components::DrawHorizontalSeparator(7.0f);
	gui_components::CreateTabBar(device);

	ImGui::End();
}

void gui_components::DrawHorizontalSeparator(float thickness) {
	ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
	float windowSizeX = ImGui::GetWindowSize().x;
	ImGui::GetWindowDrawList()->AddLine(cursorScreenPos -ImVec2(10.0f, 0.0f), ImVec2(cursorScreenPos.x + windowSizeX, cursorScreenPos.y), ImGui::GetColorU32(ImGuiCol_Separator), thickness);

	ImGui::SetCursorPosY(ImGui::GetCursorPos().y + thickness);
}

void gui_components::CreateCustomTitlebar(IDirect3DDevice9* device) {
	if (!state.logoTexture) LoadTextureFromFile(device, "C:\\Users\\madis\\source\\repos\\reaperCSGO\\Cheat\\images\\logo.png", state.logoTexture);
	ImGui::PushFont(gui::defaultFont24);

	ImGui::SetCursorPosY(TITLEBAR_LOGO_SIZE.y * 0.5f);
	ImGui::Text("Welcome, Sparkly");
	ImGui::SameLine();

	ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - TITLEBAR_LOGO_SIZE.x) * 0.5f, TITLEBAR_LOGO_SIZE.y * 0.25f)); // align image in the middle
	ImGui::Image((void*)state.logoTexture, TITLEBAR_LOGO_SIZE);
	ImGui::SameLine();
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x-ImGui::CalcTextSize("31 days remaining").x-style.WindowPadding.x, TITLEBAR_LOGO_SIZE.y * 0.5f));
	ImGui::Text("31 days remaining");

	ImGui::PopFont();
}

void gui_components::CreateTabBar(IDirect3DDevice9* device) {
	ImGui::PushFont(gui::defaultFont24);
	// TODO?: align buttons in the middle
	if (ImGui::TabbarButtonC("AIMBOT", 0, state.currentActiveTab == 0)) state.currentActiveTab = 0;
	ImGui::SameLine();
	if (ImGui::TabbarButtonC("VISUALS", 0, state.currentActiveTab == 1)) state.currentActiveTab = 1;
	ImGui::SameLine();
	if (ImGui::TabbarButtonC("MISC", 0, state.currentActiveTab == 2)) state.currentActiveTab = 2;
	ImGui::PopFont();
}