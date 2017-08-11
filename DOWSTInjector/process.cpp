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

#include "stdafx.h"

#include "Process.h"

#include <TlHelp32.h>

#include <stdio.h>
#include <string>
#include <winternl.h>

using namespace injector;

bool Process::GetAllRunningProcesses()
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    ZeroMemory(&pe32, sizeof(pe32));
    pe32.dwSize = sizeof(pe32);

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);
        return false;
    }

    do
    {
        RunningProcess rproc;
        rproc.id = pe32.th32ProcessID;
        rproc.name = std::wstring(pe32.szExeFile);
        _processes.push_back(rproc);
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    return true;
}

bool Process::FreeMemory(ptr_t address, size_t size)
{
    if (VirtualFreeEx(_procHandle, reinterpret_cast<void*>(address), size, MEM_RELEASE) == FALSE)
        return false;
    return true;
}

// NtCreateThreadEx
typedef NTSTATUS(WINAPI *fnNtCreateThreadEx)(
    OUT PHANDLE hThread,
    IN ACCESS_MASK DesiredAccess,
    IN LPVOID ObjectAttributes,
    IN HANDLE ProcessHandle,
    IN LPTHREAD_START_ROUTINE lpStartAddress,
    IN LPVOID lpParameter,
    IN BOOL CreateSuspended,
    IN DWORD StackZeroBits,
    IN DWORD SizeOfStackCommit,
    IN DWORD SizeOfStackReserve,
    OUT LPVOID lpBytesBuffer
    );

uintptr_t Process::AllocateMemory(ptr_t address, size_t size, DWORD flAllocType, DWORD flProtect)
{
    void* ptr = VirtualAllocEx(_procHandle, reinterpret_cast<void*>(address), size, flAllocType, flProtect);
    if (ptr == nullptr)
        return 0;
    return reinterpret_cast<ptr_t>(ptr);
}

bool Process::WriteMemory(ptr_t address, const void* data, size_t len)
{
    SIZE_T bytes_written;
    if (WriteProcessMemory(_procHandle, reinterpret_cast<void*>(address), data, len, &bytes_written) == FALSE)
        return false;
    if (bytes_written != len)
        return false;
    return true;
}

bool Process::CreateRemoteThread(HANDLE& hThread,
    ptr_t routine,
    ptr_t arg,
    CreateThreadFlags flags,
    DWORD access)
{
    hThread = NULL;
    static fnNtCreateThreadEx NtCreateThreadEx = nullptr;
    if (NtCreateThreadEx == nullptr) {
        HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
        NtCreateThreadEx = (fnNtCreateThreadEx)GetProcAddress(hNtdll, "NtCreateThreadEx");
    }

    NTSTATUS status = NtCreateThreadEx(&hThread,
        access,
        NULL,
        _procHandle,
        (LPTHREAD_START_ROUTINE)routine,
        (LPVOID)arg,
        (DWORD)flags,
        0,
        0x1000,
        0x100000,
        NULL);

    if (NT_SUCCESS(status))
        return true;

    return false;
}

bool Process::Open(DWORD ProcessID, DWORD Access)
{
    _procHandle = OpenProcess(Access, FALSE, ProcessID);
    if (_procHandle == NULL)
        return false;

    return true;
}

void Process::Close()
{
    if (_procHandle != NULL)
        CloseHandle(_procHandle);
}
