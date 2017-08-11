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

#include "hooks.h"
#include "imgui_dx9.h"
#include "options.h"

using namespace hooks;

HWND dowss::hWnd = NULL;

static WNDPROC oldWndProc;

bool Key::Pressed(int vkey)
{
    return ImGui::IsKeyDown(vkey);
}

static LRESULT WINAPI WndProcCB(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGuiIO& io = ImGui::GetIO();
    switch (uMsg) {
        case WM_NCACTIVATE:
            if (wParam == FALSE) {
                Options::MenuOpen = false;
            }
            break;
        case WM_KEYDOWN:
            io.KeysDown[wParam] = true;
            for (Options::ToggleKey& tk : Options::togglekeys)
            {
                if (!tk.hasmodifier || io.KeysDown[tk.modifier])
                {
                    if (io.KeysDown[tk.vkey] && !(lParam & (1 << 30)))
                        tk.enabled = !tk.enabled;
                }
            }
            break;
        case WM_KEYUP:
            io.KeysDown[wParam] = false;
            break;
        case WM_CHAR:
            // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
            if (wParam > 0 && wParam < 0x10000)
                io.AddInputCharacter((unsigned short)wParam);
            break;
        case WM_MOUSEMOVE:
            io.MousePos.x = (short)LOWORD(lParam);
            io.MousePos.y = (short)HIWORD(lParam);
            break;
        case WM_LBUTTONDOWN:
            io.MouseDown[0] = true;
            break;
        case WM_LBUTTONUP:
            io.MouseDown[0] = false;
            break;
        case WM_RBUTTONDOWN:
            io.MouseDown[1] = true;
            break;
        case WM_RBUTTONUP:
            io.MouseDown[1] = false;
            break;
        case WM_MBUTTONDOWN:
            io.MouseDown[2] = true;
            break;
        case WM_MBUTTONUP:
            io.MouseDown[2] = false;
            break;
        case WM_MOUSEWHEEL:
            io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +0.5f : -0.5f;
            break;
    }

    return CallWindowProcW(oldWndProc, hWnd, uMsg, wParam, lParam);
}

bool WndProc::Hook()
{
    auto EnumWindowsCB = [](HWND hWnd, LPARAM lParam) {
        DWORD lpdwProcessId;
        GetWindowThreadProcessId(hWnd, &lpdwProcessId);
        if (lpdwProcessId == (DWORD)lParam)
        {
            dowss::hWnd = hWnd;
            return FALSE;
        }
        return TRUE;
    };

    EnumWindows(EnumWindowsCB, GetCurrentProcessId());
    if (dowss::hWnd == NULL)
        return false;
    oldWndProc = (WNDPROC)SetWindowLongPtrW(dowss::hWnd, GWL_WNDPROC, (LONG_PTR)WndProcCB);
    if (oldWndProc == NULL)
        return false;
    return true;
}

void WndProc::UnHook()
{
    SetWindowLongPtrW(dowss::hWnd, GWL_WNDPROC, (LONG_PTR)oldWndProc);
}
