#pragma once

#include "imgui\imgui.h"

struct IDirect3DDevice9;

namespace ImGuiDX9
{
void RenderDrawLists(ImDrawData* draw_data);
bool Init(IDirect3DDevice9* device);
void Shutdown();
bool CreateDeviceObjects();
void InvalidateDeviceObjects();
void NewFrame();
}