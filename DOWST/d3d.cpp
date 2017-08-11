/*
 * Copyright (c) 2017, Kurt Cancemi (kurt@x64architecture.com)
 *
 * This file is part of DoW Soulstorm Trainer (DOWST).
 *
 *  DOWST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3
 *  as published by the Free Software Foundation.
 *
 *  DOWST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with DOWST.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdafx.h"

#include <d3d9.h>

#include "hooks.h"
#include "imgui_dx9.h"
#include "options.h"

using namespace hooks;

static bool Inited = false;

void InitGui(IDirect3DDevice9* d3d9Device)
{
    ImGuiDX9::Init(d3d9Device);
    ImGuiDX9::CreateDeviceObjects();
    Inited = true;
}

d3d::EndSceneFn d3d::oEndScene;
HRESULT __stdcall d3d::EndScene(IDirect3DDevice9* d3d9Device)
{
    if (!Inited)
        InitGui(d3d9Device);

    ImGuiDX9::NewFrame();

    if (Options::MenuOpen)
    {
        if (ImGui::Begin("DOW Soulstorm Trainer", &Options::MenuOpen, ImVec2(351, 346), -1.0f, ImGuiWindowFlags_NoCollapse))
        {
            ImGui::TextWrapped("Press the [INSERT] key to open/close this window");
            ImGui::TextWrapped("Press the [END] key to unload the trainer");
            ImGui::Spacing();
            if (ImGui::CollapsingHeader("About", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::TextWrapped("DoW Soulstorm Trainer, 1.0\n"
                                   " By Kurt Cancemi (kurt@x64architecture.com)\n\n"
                                   "DoW Soulstorm Trainer is licensed under the GPLv3 License, see LICENSE for more information.");
            }

            if (ImGui::CollapsingHeader("Options", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Columns(2, "optionsctrlcolumn", false);
                ImGui::Checkbox("Invincible Entities", &Options::InvincibleEntities);
                ImGui::Checkbox("Unlimited Requisition", &Options::UnlimitedRequisition);
                ImGui::Checkbox("Unlimited Power", &Options::UnlimitedPower);
                ImGui::Checkbox("Unlimited Units", &Options::UnlimitedUnits);
                ImGui::Checkbox("Unlimited Vehicles", &Options::UnlimitedVehicles);
                ImGui::NextColumn();
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Text("[CTRL+NUMPAD1]");
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Text("[CTRL+NUMPAD2]");
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Text("[CTRL+NUMPAD3]");
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Text("[CTRL+NUMPAD4]");
                ImGui::AlignFirstTextHeightToWidgets();
                ImGui::Text("[CTRL+NUMPAD5]");
            }
            ImGui::End();
        }

        ImGui::Render();
    }

    return oEndScene(d3d9Device);
}

d3d::ResetFn d3d::oReset;
HRESULT __stdcall d3d::Reset(IDirect3DDevice9* d3d9Device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    if (!Inited)
        return oReset(d3d9Device, pPresentationParameters);

    ImGuiDX9::InvalidateDeviceObjects();

    auto hr = oReset(d3d9Device, pPresentationParameters);
    ImGuiDX9::CreateDeviceObjects();
    return hr;
}