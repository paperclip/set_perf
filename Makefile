

CXX := g++

benchmark : benchmark.cpp
	$(CXX) -O3 -g -march=native -std=c++23 $< -o $@

benchmark_with_absl : benchmark.cpp
	$(CXX) -O3 -g -march=native -std=c++23 $< -o $@ -DWITH_ABSL -labsl_raw_hash_set -labsl_hash

unordered_set_perf : unordered_set_perf.cpp
	$(CXX) -O3 -g -march=native -std=c++23 $< -o $@
