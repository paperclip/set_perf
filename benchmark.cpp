// Copyright Douglas Leeder 2026

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <vector>

using VectorType = std::vector<uint32_t>;

#define PRINT(x) std::cerr << x << '\n'

static VectorType dedup_bitset(const VectorType& input, unsigned maxValue)
{
    boost::dynamic_bitset<> seen{ maxValue+1, 0 };
    seen.reset();
    VectorType out;

    for (uint32_t x : input) 
    {
        if (!seen.test(x)) {
            seen.set(x);
            out.push_back(x);
        }
    }
    return out;
}

static VectorType dedup_unordered_set(const VectorType& input)
{
    std::unordered_set<uint32_t> seen;
    VectorType out;

    for (const auto& x : input) 
    {
        if (seen.insert(x).second) out.push_back(x);
    }
    return out;
}

static VectorType generateInput(int n, uint32_t maxValue)
{
    static std::mt19937 randomSource;
    std::uniform_int_distribution<uint32_t> generator{0, maxValue};

    VectorType output;
    for (auto i=0; i<n; i++)
    {
        output.push_back(generator(randomSource));
    }
    return output;
}


static VectorType generateInput(int n)
{
    return generateInput(n, n*2);
}



int main()
{
    constexpr unsigned maxValue = 128*2;
    const auto input = generateInput(128, maxValue);

    using namespace ankerl::nanobench;
    auto bench = ankerl::nanobench::Bench();
    bench.relative(true);

    bench.run("unordered_set", [&] {
        auto d = dedup_unordered_set(input);
        ankerl::nanobench::doNotOptimizeAway(d);
    });
    bench.run("boost bitset", [&] {
        auto d = dedup_bitset(input, maxValue);
        ankerl::nanobench::doNotOptimizeAway(d);
    });

    return 0;
}