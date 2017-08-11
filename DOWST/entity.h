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
class Entity;
class EntityController;
class Extension;

using namespace math;

namespace gamefuncs
{
    using GetBlueprintNameFn = const char*(__thiscall*)(Entity*);
    extern GetBlueprintNameFn GetBlueprintName;
    constexpr const char kGetBlueprintNameFnName[] = "?GetBlueprintName@Entity@@QBEPBDXZ";

    using QIFn = Extension*(__thiscall*)(EntityController*, uint32_t);
    extern QIFn QI;
    constexpr const char kQIFnName[] = "?QI@EntityController@@QAEPAVExtension@@I@Z";

    using SetInvulnerableFn = void(__stdcall*)(Entity*, bool);
    extern SetInvulnerableFn SetInvulnerable;
}

class EntityController
{
public:
    Extension* QI(uint32_t cmd) { return gamefuncs::QI(this, cmd); }
    bool IsEngineer() { return QI(0xB); }
};

class Entity
{
public:
    const char* GetBlueprintName() { return gamefuncs::GetBlueprintName(this); }
    void SetInvulnerable(bool inv) { gamefuncs::SetInvulnerable(this, inv); }

    uint32_t GetID() { return _id; };
    EntityController* GetEntityController() { return _entityController; };
    Vector3f GetPosition3D() { return _position3d; }

private:
    uint8_t __pad0000[8];                // 0x0000
    uint32_t _id;                        // 0x0008
    uint8_t __pad0012[12];               // 0x000C
    EntityController* _entityController; // 0x0018
    uint8_t __pad0028[60];               // 0x001C
    Vector3f _position3d;                // 0x0054
};

class EntityGroup
{
public:
    Entity** begin() { return *MakePtr<Entity***>(this, 0x14); }

    Entity** end()
    {
        uintptr_t start = (uintptr_t)*MakePtr<Entity**>(this, 0x14);
        uintptr_t end = (uintptr_t)*MakePtr<Entity**>(this, 0x18);
        return MakePtr<Entity**>(start, (end * 4));
    }
};
}