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

// DOWSTInjector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <io.h>
#include <string>

#include "Process.h"
#include "Resource.h"

using namespace injector;

std::string GetRandomTempFileName()
{
    std::vector<char> temppath;
    temppath.resize(MAX_PATH * 2);
    GetTempPathA(temppath.size(), &temppath.data()[0]);

    char temptemplate[] = "dowssDllXXXXXX";
    strcat_s(&temppath.data()[0], temppath.size(), temptemplate);
    _mktemp_s(temppath.data(), temppath.size());
    strcat_s(&temppath.data()[0], temppath.size(), ".");

    return std::string(temppath.data());
}

int main()
{
    HANDLE hToken = NULL;
    TOKEN_PRIVILEGES priv = {};
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        priv.PrivilegeCount = 1;
        priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        if (LookupPrivilegeValueW(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid))
            AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);

        CloseHandle(hToken);
    }

    std::string dllPath = GetRandomTempFileName();
    std::cout << dllPath << std::endl;
    HRSRC resInfo = FindResourceW(NULL, MAKEINTRESOURCEW(IDR_DLL), L"DLL");
    if (resInfo)
    {
        HGLOBAL hRes = LoadResource(NULL, resInfo);
        PVOID pDLLData = LockResource(hRes);
        HANDLE hFile =
            CreateFileA(dllPath.c_str(),
                FILE_GENERIC_WRITE,
                0,
                NULL,
                CREATE_ALWAYS,
                0,
                NULL);

        if (hFile != INVALID_HANDLE_VALUE)
        {
            DWORD bytes = 0;
            WriteFile(hFile, pDLLData, SizeofResource(NULL, resInfo), &bytes, NULL);
            CloseHandle(hFile);
        }
    }

    Process process;
    std::cout << "Waiting for Soulstorm.exe..." << std::endl;
    DWORD pid = 0;
    do
    {
        if (!process.GetAllRunningProcesses())
            continue;
        for (auto& rp : process.GetProcesses())
        {
            if (rp.name.find(L"Soulstorm.exe") != std::wstring::npos)
            {
                pid = rp.id;
                break;
            }
        }
        Sleep(500);
    }
    while (!pid);
    Sleep(2000);
    std::cout << "Found! PID: " << pid << std::endl;
    if (!pid)
    {
        std::cout << "Couldn't find Soulstorm.exe" << std::endl;
        return -1;
    }
    if (!process.Open(pid, PROCESS_ALL_ACCESS))
    {
        std::cout << "Couldn't get handle to Soulstorm.exe" << std::endl;
        return -1;
    }
    ptr_t dllPathAddr = process.AllocateMemory(NULL, dllPath.size(), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!dllPathAddr)
    {
        std::cout << "Failed to allocate memory in remote process!" << std::endl;
        return -1;
    }

    if (!process.WriteMemory(dllPathAddr, dllPath.c_str(), dllPath.size()))
    {
        std::cout << "Failed to write dll path into remote process!" << GetLastError() << std::endl;
        return -1;
    }

    std::cout << std::hex << dllPathAddr << std::endl;

    void* LoadLibraryAddress = GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryA");
    HANDLE hRemoteThread;
    if (!process.CreateRemoteThread(hRemoteThread, (ptr_t)LoadLibraryAddress, dllPathAddr, NoThreadFlags, THREAD_ALL_ACCESS))
        return -1;
    WaitForSingleObject(hRemoteThread, INFINITE);
    process.FreeMemory(dllPathAddr, dllPath.size());
    DeleteFileA(dllPath.substr(dllPath.size() - 1).c_str());
    printf("Injected!\n");
    return 0;
}
