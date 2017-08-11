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

namespace Options
{
struct ToggleKey
{
    int modifier;
    int vkey;
    bool& enabled;
    bool hasmodifier;
};

extern ToggleKey togglekeys[6];

extern bool InvincibleEntities;
extern bool UnlimitedRequisition;
extern bool UnlimitedPower;
extern bool UnlimitedUnits;
extern bool UnlimitedVehicles;
extern bool MenuOpen;
}