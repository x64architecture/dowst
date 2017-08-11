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

namespace dowss
{
class CpuPlayer;
class EntityGroup;
class SquadGroup;

namespace gamefuncs
{
    using KillPlayerFn = void(__thiscall*)(Player*, int);
    extern KillPlayerFn KillPlayer;
    constexpr const char kKillPlayerFnName[] = "?KillPlayer@Player@@UAEXH@Z";
    using GetCpuPlayerFromIDFn = CpuPlayer*(__stdcall*)(int);
    extern GetCpuPlayerFromIDFn GetCpuPlayerFromID;
}

class Player
{
public:
    bool IsCPU() { return gamefuncs::GetCpuPlayerFromID(_id) != nullptr; }
    void KillPlayer() { gamefuncs::KillPlayer(this, 0 /*??*/); }

    const wchar_t* GetName() { return _name; }
    uint32_t GetID() { return _id; };
    EntityGroup* GetEntities() { return _entities; }
    float* GetRequisition() { return &_requisition; }
    float* GetPower() { return &_power; }
    int* GetUnits() { return &_units; }
    int* GetVehicles() { return &_vehicles; }

private:
    uint8_t __pad_0000[4];        // 0x0000
    uint32_t _id;                 // 0x0004
    EntityGroup* _entities;       // 0x0008
    SquadGroup* _squads;          // 0x000C
    wchar_t* _name;               // 0x0010
    uint8_t __pad_0014[8];        // 0x0014
    bool _dead;                   // 0x001C
    uint8_t __pad_001D[43];       // 0x001D
    bool _forceSwitchTeamColours; // 0x0048
    bool _isDarkEldar;            // 0x0049
    uint8_t __pad_004A[426];      // 0x004A
    float _requisition;           // 0x01F4
    float _power;                 // 0x01F8
    uint8_t __pad_01FC[76];       // 0x01FC
    int32_t _units;               // 0x0248
    int32_t _vehicles;            // 0x024C

};

}