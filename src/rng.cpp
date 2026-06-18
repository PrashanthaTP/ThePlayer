#include <chrono>
#include <iostream>
#include <random>

#include "rng.hpp"

// SEE : Static Initialization Order Fiasco (SIOF).
// unsigned int seed =
// std::chrono::high_resolution_clock::now().time_since_epoch().count();
// std::mt19937 Rng::_gen{std::random_device{"/dev/urandom"}()}; //freezes
// std::mt19937 Rng::_gen{seed};

int Rng::getRandom(int min, int max) {

    int temp_min{min};
    int temp_max{max};
    min = std::min(temp_min, temp_max);
    max = std::max(temp_min, temp_max);
    static std::mt19937 gen{[] {
        unsigned int seed = std::chrono::high_resolution_clock::now()
                                .time_since_epoch()
                                .count();
        return std::mt19937(seed);
    }()};

    std::uniform_int_distribution<int> distr(min, max);
    int val = distr(gen);
    return val;
}
