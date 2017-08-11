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

#pragma once

#include <stdint.h>
#include <string.h>
#include <vector>

/*
 * Copyright (c) 2016-2017 Kurt Cancemi <kurt@x64architecture.com>
 */
/*
 * Virtual Method Table Hooking Class For MSVC
 * ===========================================
 *
 * Brief:
 *   Throughout many C++ game engines the use of the virtual keyword in
 *   classes creates a pointer to a (V)irtual (M)ethod (T)able (VMT)
 *   located in the beginning segment of the class memory which contains
 *   function pointers to the functions marked with the virtual keyword.
 *
 * /d1reportSingleClassLayoutAA
 *
 * class AA {
 * public:
 *     int x;
 *     virtual void X();
 *     virtual int Y();
 *     void Z();
 * };
 *
 * class AA        size(8):
 *         +---
 *  0      | {vfptr}
 *  4      | x
 *         +---
 * 
 * AA::$vftable@:
 *         | &AA_meta
 *         |  0
 *  0      | &AA::X
 *  1      | &AA::Y
 *
 */

// Directly modifies .rdata so probably unwise to use in a ac-protected game
class VMTHook {
public:
    VMTHook() = delete;
    VMTHook(uintptr_t** vmt) : _vmt(*vmt)
    {
    }

    template <typename T>
    T AssignVMTFunc(uintptr_t new_function_addr, size_t index)
    {
        struct vmt_function_record record;

        record.address = _vmt[index];
        record.index = index;
        VirtualProtect((void*)(_vmt[index]), sizeof(uintptr_t), PAGE_READWRITE, &record.oldprotflags), 
        _records.push_back(record);

        _vmt[index] = new_function_addr;

        return (T)record.address;
    }

    ~VMTHook()
    {
        for (auto& record : _records)
        {
            DWORD oldprotflags;
            _vmt[record.index] = record.address;
            VirtualProtect((void*)(_vmt[record.index]), sizeof(uintptr_t), record.oldprotflags, &oldprotflags);
        }
        _vmt = nullptr;
    }

public:
    uintptr_t* _vmt;
private:
    struct vmt_function_record
    {
        uintptr_t address;  // Original function address
        size_t index;       // VMT index
        DWORD oldprotflags; // Old memory protection flags
    };
    std::vector<vmt_function_record> _records;
};
