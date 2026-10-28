#pragma once

#include <cstdint>

namespace rnd {
    float random();

    int randint(int a, int b);

    float uniform(float a, float b);

    void seed(uint32_t value);
}