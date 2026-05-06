// Copyright Douglas Leeder 2026

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include <bitset>
#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <vector>

using VectorType = std::vector<uint32_t>;

#define PRINT(x) std::cerr << x << '\n'

static VectorType dedup_boost_dynamic_bitset(const VectorType& input, unsigned maxValue)
{
    boost::dynamic_bitset<> seen{ maxValue, 0 };
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

template<std::size_t MAX_VALUE>
static VectorType dedup_std_bitset_heap(const VectorType& input)
{
    std::unique_ptr<std::bitset<MAX_VALUE>> seen = std::make_unique<std::bitset<MAX_VALUE>>();
    VectorType out;

    for (uint32_t x : input) 
    {
        if (!seen->test(x)) {
            seen->set(x);
            out.push_back(x);
        }
    }
    return out;
}

template<std::size_t MAX_VALUE>
static VectorType dedup_std_bitset_stack(const VectorType& input)
{
    static_assert(MAX_VALUE <= 1'000'000, "MAX_VALUE is too large for stack allocation");
    std::bitset<MAX_VALUE> seen;
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

static VectorType dedup_unordered_set_reserve(const VectorType& input)
{
    std::unordered_set<uint32_t> seen;
    seen.reserve(input.size());
    VectorType out;
    out.reserve(input.size());

    for (const auto& x : input) 
    {
        if (seen.insert(x).second) out.push_back(x);
    }
    return out;
}

static VectorType dedup_sort_unique(const VectorType& input)
{
    auto output = input;
    std::sort(output.begin(), output.end());
    auto end = std::unique(output.begin(), output.end());
    output.erase(end, output.end());
    return output;
}

/////////////////////////////////////////////////////////////////////////////////

static VectorType generateInput(int n, uint32_t maxValue)
{
    static std::mt19937 randomSource;
    std::uniform_int_distribution<uint32_t> generator{0, maxValue-1};

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

template<std::size_t n, std::size_t maxValue>
static void runTest(int minEpochIterations = 10)
{
    PRINT("\nTesting " << n << " from " << maxValue);
    
    const auto input = generateInput(n, maxValue);

    using namespace ankerl::nanobench;
    auto bench = ankerl::nanobench::Bench();
    bench.relative(true);
    bench.minEpochIterations(minEpochIterations);

    bench.run("unordered_set", [&] {
        ankerl::nanobench::doNotOptimizeAway(dedup_unordered_set(input));
    });
    bench.run("unordered_set_reserve", [&] {
        ankerl::nanobench::doNotOptimizeAway(dedup_unordered_set_reserve(input));
    });
    bench.run("boost dynamic bitset", [&] {
        ankerl::nanobench::doNotOptimizeAway(dedup_boost_dynamic_bitset(input, maxValue));
    });
    bench.run("std::bitset", [&] {
        ankerl::nanobench::doNotOptimizeAway(dedup_std_bitset_heap<maxValue>(input));
    });
    if constexpr (maxValue <= 1'000'000) 
    {
        bench.run("std::bitset stack", [&] {
            ankerl::nanobench::doNotOptimizeAway(dedup_std_bitset_stack<maxValue>(input));
        });
    }
    bench.run("sort_unique", [&] {
        ankerl::nanobench::doNotOptimizeAway(dedup_sort_unique(input));
    });
}

template<std::size_t n>
static void runTest()
{
    constexpr auto maxValue = n*2;
    runTest<n, maxValue>();
}

int main()
{
    runTest<128>();
    runTest<2048>();
    runTest<32768>();
    runTest<131072>();
    runTest<500000>();

    runTest<8, 100'000'000>(100);
    runTest<128, 100'000'000>(100);
    runTest<2048, 100'000'000>(100);
    runTest<8192, 100'000'000>(100);
    runTest<32768, 100'000'000>(100);
    runTest<65536, 100'000'000>(100);

    return 0;
}