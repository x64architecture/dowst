/*
 * Copyright (c) 2017, Kurt Cancemi (kurt@x64architecture.com)
 *
 * This file is part of DoW Soulstorm Trainer (DOWSST).
 *
 *  DOWSST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3
 *  as published by the Free Software Foundation.
 *
 *  DOWSST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with DOWSST.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <vector>

typedef unsigned long DWORD;

namespace injector
{

using ptr_t = uintptr_t;

struct RunningProcess
{
    DWORD id;
    std::wstring name;
};

enum CreateThreadFlags
{
    NoThreadFlags = 0x0000,
    CreateSuspended = 0x0001,
    NoDllCallbacks = 0x0002,
    HideFromDebug = 0x0004,
};

class Process
{
public:
    bool Open(DWORD ProcessID, DWORD Access);
    void Close();
    bool GetAllRunningProcesses();
    bool FreeMemory(ptr_t address, size_t size);
    bool CreateRemoteThread(HANDLE& hThread, ptr_t routine, ptr_t arg, CreateThreadFlags flags, DWORD access);
    uintptr_t AllocateMemory(ptr_t address, size_t size, DWORD flAllocType, DWORD flProtect);
    bool WriteMemory(ptr_t address, const void* data, size_t len);

    std::vector<RunningProcess> GetProcesses() { return _processes; };
private:
    HANDLE _procHandle;
    std::vector<RunningProcess> _processes;
};
}