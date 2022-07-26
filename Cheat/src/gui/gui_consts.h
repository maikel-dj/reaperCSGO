#pragma once
#include "../includes.h"

// ImGui styling & drawing settings
// Menu window
const ImVec2 MAIN_MENU_SIZE = ImVec2(700.0f, 700.0f);
const ImGuiWindowFlags MAIN_MENU_FLAGS = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;

// Title bar
const float TITLEBAR_HEIGHT = 60.0f;
const float TITLEBAR_FONT_SIZE = 45.0f;
const ImVec2 TITLEBAR_LOGO_SIZE = ImVec2(130.0f, 49.0f); // y/x 0.378539493293592

// Sidebar
const float SIDEBAR_WIDTH = 250.0f;