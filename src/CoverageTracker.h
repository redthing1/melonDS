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

#ifndef COVERAGETRACKER_H
#define COVERAGETRACKER_H

#include <unordered_map>

#include "types.h"

namespace melonDS
{
struct CoverageSnapshot
{
    std::unordered_map<u32, u64> Arm9;
    std::unordered_map<u32, u64> Arm7;
};

class CoverageTracker
{
public:
    void Start() noexcept;
    [[nodiscard]] bool IsActive() const noexcept;
    bool Stop(CoverageSnapshot& out);
    void Restore(CoverageSnapshot&& snapshot);
    void Record(u32 cpuNum, u32 blockAddr) noexcept;

private:
    bool Active = false;
    std::unordered_map<u32, u64> Hits[2];
    u32 LastAddr[2] = {0, 0};
    u64* LastCounter[2] = {nullptr, nullptr};

    void ResetCaches() noexcept;
};
}

#endif // COVERAGETRACKER_H
