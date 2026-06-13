#include <random>


class Rng {
private:
    static std::mt19937 _gen;
public:
    int getRandom(int min, int max);

};
