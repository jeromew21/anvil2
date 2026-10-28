#include "random.hpp"

#include <chrono>
#include <random>

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

void seed(uint32_t value) { rng_instance().engine.seed(value); }

float random() {
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(rng_instance().engine);
}

int randint(int a, int b) {
    std::uniform_int_distribution<int> dist(a, b);
    return dist(rng_instance().engine);
}

float uniform(float a, float b) {
    std::uniform_real_distribution<float> dist(a, b);
    return dist(rng_instance().engine);
}
} // namespace rnd