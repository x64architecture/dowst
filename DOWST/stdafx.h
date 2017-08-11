// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define _WIN32_WINNT 0x601
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here

#include <stdint.h>

template<typename T>
T MakePtr(uintptr_t base, uintptr_t offset = 0)
{
    return reinterpret_cast<T>(base + offset);
}

template<typename T>
T MakePtr(const void* base, uintptr_t offset = 0)
{
    return reinterpret_cast<T>((uintptr_t)base + offset);
}

#include "console.h"
#include "signatures.h"
#include "key.h"