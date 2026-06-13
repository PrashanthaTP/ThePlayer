#include "rng.hpp"


std::mt19937 Rng::_gen{std::random_device{}()};

int Rng::getRandom(int min, int max){
    std::uniform_int_distribution<int> distr(min, max);
    return distr(_gen);
}
