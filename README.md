# set_perf
Investigate performance of unordered_set and alternatives

Develops ideas from https://dubeykartikay.com/posts/why-never-use-std-unordered-set/

Tests
=====

| relative |               ns/op |                op/s |    err% |          ins/op |          cyc/op |    IPC |         bra/op |   miss% |     total | benchmark                                                 |---------:|--------------------:|--------------------:|--------:|----------------:|----------------:|-------:|---------------:|--------:|----------:|:----------
|   100.0% |            2,490.42 |          401,538.57 |    1.5% |       35,723.01 |       10,847.16 |  3.293 |       7,261.00 |    0.0% |      0.01 | `unordered_set`
|    67.2% |            3,707.59 |          269,717.07 |    2.0% |       49,755.29 |       16,106.42 |  3.089 |      10,475.95 |    0.3% |      0.01 | `unordered_set_reserve`
|   647.9% |              384.41 |        2,601,388.62 |    1.3% |        6,167.00 |        1,679.42 |  3.672 |       1,114.00 |    0.0% |      0.01 | `boost dynamic bitset`
|   957.2% |              260.18 |        3,843,429.51 |    2.4% |        4,476.00 |        1,128.53 |  3.966 |         918.00 |    0.0% |      0.01 | `std::bitset`
| 1,017.3% |              244.81 |        4,084,849.27 |    2.5% |        4,188.00 |        1,053.04 |  3.977 |         881.00 |    0.0% |      0.01 | `std::bitset stack`
|   343.5% |              724.96 |        1,379,393.59 |    0.6% |        8,374.00 |        3,162.36 |  2.648 |       1,730.00 |    0.0% |      0.01 | `sort_unique`

unordered_set
-------------

Uses std::unordered_set to eliminate duplicates

unordered_set_reserve
---------------------

Reserves sufficient space in both unordered_set and the output vector before starting.

boost dynamic bitset
--------------------

Uses boost dynamic bitset 

std::bitset heap
----------------

Uses std::bitset on the heap (std::unique_ptr)

std::bitset stack
-----------------

Uses std::bitset on the stack, but limits max_size to avoid blowing the stack

sort_unique
-----------

Sorts the Vector, then runs unique on it, then trims the vector.
Could be even faster if destructive, but this test copies the vector first.

ranges_sort_unique
------------------

Use std::ranges::sort and std::ranges::unique instead of std::sort and std::unique.
Looks to be slightly slower in most cases?

plf::bitset heap
----------------

Use https://github.com/mattreecebentley/plf_bitsets stored on the heap.
Download plf_bitset.h to this directory to use.

plf::bitset stack
-----------------

For smaller MAX_SIZE, uses plf::bitset on the stack.

