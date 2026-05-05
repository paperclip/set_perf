

benchmark : benchmark.cpp
	g++ -O3 -g -march=native -std=c++20 $< -o $@

unordered_set_perf : unordered_set_perf.cpp
	g++ -O3 -g -march=native -std=c++20 $< -o $@
