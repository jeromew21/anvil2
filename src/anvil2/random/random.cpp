#include "random.hpp"

#include <chrono>
#include <random>
#include <cassert>

namespace {
struct RandomNumberGenerator {
    std::mt19937 engine;

    RandomNumberGenerator() {
        auto seed =
            static_cast<uint32_t>(std::chrono::high_resolution_clock::now()
                                      .time_since_epoch()
                                      .count());
        engine.seed(seed);
    }
};

RandomNumberGenerator &rng_instance() {
    static RandomNumberGenerator instance; // initialized on first use
    return instance;
}
} // namespace

namespace rnd {

/**
 * @brief Seed the random number generator with a specific value.
 * @param value The seed value.
 */
void seed(uint32_t value) { rng_instance().engine.seed(value); }

/**
 * @brief Generate a random floating-point number in the range [0, 1).
 * @return A random float between 0 and 1.
 */
float random() {
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(rng_instance().engine);
}

/**
 * @brief Generate a random integer in the range [a, b).
 * @param a The lower bound (inclusive).
 * @param b The upper bound (exclusive).
 * @pre a must be less than b.
 * @return A random integer between a and b-1.
 */
int randint(int a, int b) {
    assert(a < b);
    std::uniform_int_distribution<int> dist(a, b - 1);
    return dist(rng_instance().engine);
}

/**
 * @brief Generate a random integer in the range [0, a).
 * @param a The upper bound (exclusive).
 * @pre a must be greater than 0.
 * @return A random integer between 0 and a-1.
 */
int randint(int a) {
    assert(a > 0);
    return randint(0, a);
}

/**
 * @brief Generate a random floating-point number in the range [a, b).
 * @param a The lower bound (inclusive).
 * @param b The upper bound (exclusive).
 * @return A random float between a and b.
 */
float uniform(float a, float b) {
    std::uniform_real_distribution<float> dist(a, b);
    return dist(rng_instance().engine);
}
} // namespace rnd