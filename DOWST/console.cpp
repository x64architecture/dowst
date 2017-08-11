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

#include <stdio.h>

#if defined(_DEBUG)

bool Console::Setup()
{
    if (!AllocConsole())
        return false;

    FILE* err;
    FILE* out;
    if (freopen_s(&out, "CONOUT$", "w", stdout) != 0)
        return false;
    if (freopen_s(&err, "CONOUT$", "w", stderr) != 0)
        return false;

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

    return true;
}

void Console::Cleanup()
{
    fclose(stdout);
    fclose(stderr);
    FreeConsole();
}

void Console::Print(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}

void Console::Print(const wchar_t* format, ...)
{
    va_list ap;
    va_start(ap, format);
    vwprintf(format, ap);
    va_end(ap);
}

#else

bool Console::Setup()
{
    return true;
}

void Console::Cleanup() {}

#endif
