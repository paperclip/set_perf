

CXX := g++

benchmark : benchmark.cpp
	$(CXX) -O3 -g -march=native -std=c++23 $< -o $@

unordered_set_perf : unordered_set_perf.cpp
	$(CXX) -O3 -g -march=native -std=c++23 $< -o $@
