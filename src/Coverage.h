#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

struct CoverageTrace
{
    std::unordered_map<uint32_t, size_t> block_hits;

    void hit(uint32_t block);
    size_t total_block_count() const;
    size_t total_hit_count() const;

    static CoverageTrace merge(const CoverageTrace &trace1, const CoverageTrace &trace2);
    static void write(std::ostream &os, const CoverageTrace &trace);
};

class Coverage
{
public:
    Coverage();

    void new_trace();
    void start_tracing();
    void stop_tracing();
    void commit_trace();
    const std::vector<CoverageTrace> &get_traces() const;

    CoverageTrace *curr_trace;
    bool is_tracing;

private:
    std::vector<CoverageTrace> traces;
};
