#pragma once
#include "imgui.h"

void Style()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 5.0f;
	style.FrameRounding = 4.0f;
	style.ScrollbarRounding = 8.0f;
	style.GrabRounding = 3.0f;
	style.WindowRounding = 0.f;

	style.Colors[ImGuiCol_WindowBg] = globals.col_WindowBg_CUSTOM;
	style.Colors[ImGuiCol_Button] = globals.col_Button_CUSTOM;
	style.Colors[ImGuiCol_ButtonHovered] = globals.col_ButtonHovered_CUSTOM;
	style.Colors[ImGuiCol_ButtonActive] = globals.col_ButtonActive_CUSTOM;
	style.Colors[ImGuiCol_CheckMark] = globals.col_CheckMark_CUSTOM;
	style.Colors[ImGuiCol_SliderGrab] = globals.col_SliderGrab_CUSTOM;
	style.Colors[ImGuiCol_SliderGrabActive] = globals.col_SliderGrabActive_CUSTOM;
	style.Colors[ImGuiCol_FrameBg] = globals.col_FrameBg_CUSTOM;
	style.Colors[ImGuiCol_FrameBgHovered] = globals.col_FrameBgHovered_CUSTOM;
	style.Colors[ImGuiCol_FrameBgActive] = globals.col_FrameBgActive_CUSTOM;
	style.Colors[ImGuiCol_Header] = globals.col_Header_CUSTOM;
	style.Colors[ImGuiCol_HeaderHovered] = globals.col_HeaderHovered_CUSTOM;
	style.Colors[ImGuiCol_HeaderActive] = globals.col_HeaderActive_CUSTOM;
	style.Colors[ImGuiCol_Text] = globals.col_Text_CUSTOM;
}

void StyleDefault()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 5.0f;
	style.FrameRounding = 4.0f;
	style.ScrollbarRounding = 8.0f;
	style.GrabRounding = 3.0f;
	style.WindowRounding = 0.f;

	style.Colors[ImGuiCol_WindowBg] = ImColor(17, 17, 17, 240);
	style.Colors[ImGuiCol_Button] = ImVec4(0.86f, 0.26f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.06f, 0.85f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.96f, 0.26f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.86f, 0.24f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.26f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.17f, 0.22f, 0.54f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.00f, 0.96f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.72f, 0.26f, 0.98f, 0.65f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.98f, 0.26f, 0.90f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.89f, 0.26f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.70f, 0.26f, 0.98f, 1.00f);
}