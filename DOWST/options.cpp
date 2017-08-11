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
#include "options.h"
#include "dowss.h"

using namespace dowss;
using namespace Options;

bool Options::MenuOpen = true;
bool Options::InvincibleEntities = false;
bool Options::UnlimitedRequisition = false;
bool Options::UnlimitedPower = false;
bool Options::UnlimitedUnits = false;
bool Options::UnlimitedVehicles = false;

ToggleKey Options::togglekeys[] =
{
    { 0,          VK_INSERT,  MenuOpen, false },
    { VK_CONTROL, VK_NUMPAD1, InvincibleEntities, true },
    { VK_CONTROL, VK_NUMPAD2, UnlimitedRequisition, true },
    { VK_CONTROL, VK_NUMPAD3, UnlimitedPower, true },
    { VK_CONTROL, VK_NUMPAD4, UnlimitedUnits, true },
    { VK_CONTROL, VK_NUMPAD5, UnlimitedVehicles, true },
};