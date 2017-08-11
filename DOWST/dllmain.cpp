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


// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "hooks.h"
#include "options.h"
#include "imgui_dx9.h"

using namespace dowss;

Player* GetLocalPlayer()
{
    for (unsigned int i = 0; i < (*world)->GetPlayerCount(); i++)
    {
        Player* player = (*world)->GetPlayerAt(i);
        if (!player->IsCPU())
            return player;
    }
    return nullptr;
}

void cheat_loop()
{
    localplayer = GetLocalPlayer();
    if (localplayer != nullptr)
    {
        if (Options::UnlimitedRequisition)
            *localplayer->GetRequisition() = 9999999.f;
        if (Options::UnlimitedPower)
            *localplayer->GetPower() = 9999999.f;
        if (Options::UnlimitedUnits)
            *localplayer->GetUnits() = 0;
        if (Options::UnlimitedVehicles)
            *localplayer->GetVehicles() = 0;

        EntityGroup* entity_group = localplayer->GetEntities();
        if (entity_group == nullptr)
            return;
        for (Entity** entity = entity_group->begin(); entity != entity_group->end(); entity++)
        {
            (*entity)->SetInvulnerable(Options::InvincibleEntities);
        }
    }
}

void main_loop()
{
    while (!GetModuleHandleW(L"WXPMod.dll") || !GetModuleHandleW(L"d3d9.dll"))
        Sleep(3000);
    if (!Console::Setup() || !gamefuncs::Load())
        return;

    while (dx9device->_d3d9Device == nullptr)
        Sleep(25);

    VMTHook Dx9DeviceHook = VMTHook((uintptr_t**)dx9device->_d3d9Device);
    hooks::d3d::oEndScene = Dx9DeviceHook.AssignVMTFunc<hooks::d3d::EndSceneFn>((uintptr_t)hooks::d3d::EndScene, 42);
    hooks::d3d::oReset = Dx9DeviceHook.AssignVMTFunc<hooks::d3d::ResetFn>((uintptr_t)hooks::d3d::Reset, 16);
    while (!Key::Pressed(VK_END))
    {
        if (*world != nullptr)
            cheat_loop();
        Sleep(25);
    }
}

DWORD WINAPI DOWSSDllThread(LPVOID hModule)
{
    main_loop();

    Console::Cleanup();
    hooks::WndProc::UnHook();
    ImGuiDX9::Shutdown();
    Sleep(250);
    FreeLibraryAndExitThread((HMODULE)hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            if (!hooks::WndProc::Hook())
                return FALSE;
            CreateThread(NULL, 0, DOWSSDllThread, hModule, 0, NULL);
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
