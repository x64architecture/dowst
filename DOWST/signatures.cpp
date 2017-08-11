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

#include <vector>
#include <algorithm>

#include <Psapi.h>

/*
* This file implements signature scaning.
*/

/*
 * Convert hex to number
 * Optimized for speed (assumes valid input)
 *
 * Supported hex digits:
 *  a...f, A...F, 0...9
 */
static uint8_t hex_digit(char h)
{
    if (h >= '0' && h <= '9')
        return h - '0';
    else
        return (h & ~0x20) - 'A' + 10;
}

constexpr uint8_t wildcard = 0;

/*
 * Convert hex string to bytes
 * Assumes valid input
 *
 */
static bool HexStringToBytes(const char* signature, std::vector<uint8_t>& out_bytes)
{
    for (const char* p = signature; *p != '\0'; p += 2)
    {
        if (*p == ' ')
            ++p;

        //printf(" [%c,%c]", *p, *(p + 1));

        if (*(int16_t*)p == '??')
            out_bytes.push_back(wildcard);
        else
            out_bytes.push_back(hex_digit(*p) << 4 | hex_digit(*(p + 1)));
    }
    return true;
}

static uintptr_t FindPattern(uintptr_t base_address, uintptr_t end_address, const char* signature)
{
    std::vector<uint8_t> bytes;

    if (!HexStringToBytes(signature, bytes))
        return 0;

    const uint8_t* start = (const uint8_t*)base_address;
    const uint8_t* end = (const uint8_t*)end_address - bytes.size();

    auto search = std::search(start, end, bytes.cbegin(), bytes.cend(), [](uint8_t val1, uint8_t val2) { return (val1 == val2 || val2 == wildcard); });
    if (search == end)
        return 0;

    return (uintptr_t)search;
}

bool SigScanner::Init(const wchar_t* module_name)
{
    uintptr_t baseaddress;
    MODULEINFO modinfo = {};

    GetModuleInformation(GetCurrentProcess(), GetModuleHandleW(module_name), &modinfo, sizeof(modinfo));
    baseaddress = (uintptr_t)modinfo.lpBaseOfDll;

    auto dosHdr = MakePtr<IMAGE_DOS_HEADER*>(baseaddress);
    if (dosHdr->e_magic != IMAGE_DOS_SIGNATURE)
    {
        _dprintf("dosHdr->e_magic invalid\n");
        return false;
    }
    auto ntHdr = MakePtr<IMAGE_NT_HEADERS*>(baseaddress, dosHdr->e_lfanew);
    if (ntHdr->Signature != IMAGE_NT_SIGNATURE) {
        _dprintf("ntHdr->Signature invalid\n");
        return false;
    }

    IMAGE_SECTION_HEADER* section = IMAGE_FIRST_SECTION(ntHdr);
    for (WORD i = 0; i < ntHdr->FileHeader.NumberOfSections; ++i, ++section)
    {
        if (memcmp(section->Name, ".text\0\0\0", 8) == 0)
        {
            _textStartAddress = baseaddress + section->VirtualAddress;
            _textEndAddress = _textStartAddress + section->Misc.VirtualSize;
            return true;
        }
    }

     _dprintf("Failed to find .text section\n");

    return false;
}

uintptr_t SigScanner::FindPattern(const char* pattern)
{
    return ::FindPattern(_textStartAddress, _textEndAddress, pattern);
}
