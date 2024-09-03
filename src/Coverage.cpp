#include "Coverage.h"

#include <iostream>
#include <sstream>

void CoverageTrace::hit(uint32_t block)
{
    block_hits[block]++;
}

size_t CoverageTrace::total_block_count() const
{
    return block_hits.size();
}

size_t CoverageTrace::total_hit_count() const
{
    size_t total_hits = 0;
    for (const auto &entry : block_hits)
    {
        total_hits += entry.second;
    }
    return total_hits;
}

Coverage::Coverage() : curr_trace(nullptr), is_tracing(false) {}

void Coverage::new_trace()
{
    traces.emplace_back();
    curr_trace = &traces.back();
}

void Coverage::start_tracing()
{
    is_tracing = true;
}

void Coverage::stop_tracing()
{
    is_tracing = false;
}

void Coverage::commit_trace()
{
    curr_trace = nullptr;
}

const std::vector<CoverageTrace> &Coverage::get_traces() const
{
    return traces;
}

CoverageTrace CoverageTrace::merge(const CoverageTrace &trace1, const CoverageTrace &trace2)
{
    CoverageTrace merged_trace;
    for (const auto &entry : trace1.block_hits)
    {
        merged_trace.block_hits[entry.first] += entry.second;
    }
    for (const auto &entry : trace2.block_hits)
    {
        merged_trace.block_hits[entry.first] += entry.second;
    }
    return merged_trace;
}

void CoverageTrace::write(std::ostream &os, const CoverageTrace &trace)
{
    char buffer[64] = {0};

    // write in format "{addr:%08x} {hits:%zu}\n"
    for (const auto &entry : trace.block_hits)
    {
        sprintf(buffer, "%08x %zu\n", entry.first, entry.second);
        os << buffer;
    }
}
