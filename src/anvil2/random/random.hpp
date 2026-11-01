/** 
 * @file random.hpp
 * @brief Random number generation utilities.
 * Not cryptographically secure.
 */
#pragma once

#include <cstdint>

namespace rnd {
    float random();

    int random_integer(int a, int b);

    int random_integer(int a);

    float uniform(float a, float b);

    void seed(uint32_t value);
}