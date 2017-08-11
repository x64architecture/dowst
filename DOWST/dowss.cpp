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

#include <stdlib.h>

#include "dowss.h"


using namespace dowss;
using namespace gamefuncs;

GetPlayerAtFn gamefuncs::GetPlayerAt;
SetGameOverFn gamefuncs::SetGameOver;
KillPlayerFn gamefuncs::KillPlayer;
QIFn gamefuncs::QI;
GetBlueprintNameFn gamefuncs::GetBlueprintName;
SetInvulnerableFn gamefuncs::SetInvulnerable;
GetCpuPlayerFromIDFn gamefuncs::GetCpuPlayerFromID;
GetDllInterfaceFn gamefuncs::GetDllInterface;

Player* dowss::localplayer;
World** dowss::world;
Dx9Device* dowss::dx9device;

template<typename T>
static T GetFnAddress(HMODULE hModule, const char* name)
{
    return reinterpret_cast<T>(GetProcAddress(hModule, name));
}

bool gamefuncs::Load()
{
    HMODULE hSimEngine = GetModuleHandleW(L"SimEngine.dll");
    GetPlayerAt = GetFnAddress<GetPlayerAtFn>(hSimEngine, kGetPlayerAtFnName);
    SetGameOver = GetFnAddress<SetGameOverFn>(hSimEngine, kSetGameOverFnName);
    KillPlayer = GetFnAddress<KillPlayerFn>(hSimEngine, kKillPlayerFnName);
    QI = GetFnAddress<QIFn>(hSimEngine, kQIFnName);
    GetBlueprintName = GetFnAddress<GetBlueprintNameFn>(hSimEngine, kGetBlueprintNameFnName);

    HMODULE hWXPMod = GetModuleHandleW(L"WXPMod.dll");
    GetDllInterface = GetFnAddress<GetDllInterfaceFn>(hWXPMod, kGetDllInterfaceFnName);

    SigScanner WXPModScanner;
    if (!WXPModScanner.Init(L"WXPMod.dll"))
    {
        _dprintf("Failed to init SimEngine scanner!\n");
        return false;
    }

    SetInvulnerable = (SetInvulnerableFn)WXPModScanner.FindPattern("8B 44 24 04 85 C0 74 4C");
    GetCpuPlayerFromID = (GetCpuPlayerFromIDFn)WXPModScanner.FindPattern("8B 44 24 04 8B 4C 24 F4");
    world = *MakePtr<World***>(WXPModScanner.FindPattern("89 35 ?? ?? ?? ?? 8B C6"), 2);

    SigScanner SoulstormScanner;
    if (!SoulstormScanner.Init(L"Soulstorm.exe"))
    {
        _dprintf("Failed to init Soulstorm scanner!\n");
        return false;
    }

    dx9device = **MakePtr<Dx9Device***>(SoulstormScanner.FindPattern("8B 0D ?? ?? ?? ?? 85 C9 75 0F"), 2);


    if (GetPlayerAt == nullptr || SetGameOver == nullptr || KillPlayer == nullptr || QI == nullptr ||
        GetBlueprintName == nullptr || SetInvulnerable == nullptr || GetCpuPlayerFromID == nullptr ||
        world == nullptr || dx9device == nullptr)
    {
        abort();
    }

    return true;
}