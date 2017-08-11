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

struct IDirect3DDevice9;
typedef struct _D3DPRESENT_PARAMETERS_ D3DPRESENT_PARAMETERS;

namespace hooks
{
namespace d3d
{
    using EndSceneFn = HRESULT(__stdcall*)(IDirect3DDevice9*);
    extern EndSceneFn oEndScene;
    HRESULT __stdcall EndScene(IDirect3DDevice9*);
    using ResetFn = HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
    extern ResetFn oReset;
    HRESULT __stdcall Reset(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
}
}