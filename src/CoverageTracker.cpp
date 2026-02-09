/*
    Copyright 2016-2024 melonDS team

    This file is part of melonDS.

    melonDS is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.

    melonDS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with melonDS. If not, see http://www.gnu.org/licenses/.
*/

#include "CoverageTracker.h"

namespace melonDS
{
void CoverageTracker::ResetCaches() noexcept
{
    LastAddr[0] = 0;
    LastAddr[1] = 0;
    LastCounter[0] = nullptr;
    LastCounter[1] = nullptr;
}

void CoverageTracker::Start() noexcept
{
    Hits[0].clear();
    Hits[1].clear();
    ResetCaches();
    Active = true;
}

bool CoverageTracker::IsActive() const noexcept
{
    return Active;
}

bool CoverageTracker::Stop(CoverageSnapshot& out)
{
    if (!Active)
        return false;

    out.Arm9 = std::move(Hits[0]);
    out.Arm7 = std::move(Hits[1]);
    Hits[0].clear();
    Hits[1].clear();
    ResetCaches();
    Active = false;
    return true;
}

void CoverageTracker::Restore(CoverageSnapshot&& snapshot)
{
    Hits[0] = std::move(snapshot.Arm9);
    Hits[1] = std::move(snapshot.Arm7);
    ResetCaches();
    Active = true;
}

void CoverageTracker::Record(u32 cpuNum, u32 blockAddr) noexcept
{
    if (!Active || cpuNum > 1)
        return;

    if (LastCounter[cpuNum] != nullptr && LastAddr[cpuNum] == blockAddr)
    {
        (*LastCounter[cpuNum])++;
        return;
    }

    auto [it, inserted] = Hits[cpuNum].try_emplace(blockAddr, 0);
    it->second++;
    LastAddr[cpuNum] = blockAddr;
    LastCounter[cpuNum] = &it->second;
}
}
