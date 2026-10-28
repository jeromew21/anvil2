#pragma once

#include <utility>
#include <type_traits>

/**
 * @brief A utility class that executes a callback when it goes out of scope.
 *
 * This class is useful for implementing RAII-style deferred execution.
 * The callback is executed unless the Defer object is dismissed.
 *
 * @tparam F The type of the callback function.
 */
template<typename F>
class Defer {
public:
    explicit Defer(F &&f) noexcept(std::is_nothrow_move_constructible_v<F>)
        : callback(std::forward<F>(f)), active(true) {
    }

    Defer(Defer &&other) noexcept(std::is_nothrow_move_constructible_v<F>)
        : callback(std::move(other.callback)), active(other.active) {
        other.active = false;
    }

    Defer(const Defer &) = delete;

    Defer &operator=(const Defer &) = delete;

    Defer &operator=(Defer &&) = delete;

    ~Defer() noexcept {
        if (active) {
            callback();
        }
    }

    /**
     * @brief Dismiss the deferred execution.
     *
     * After calling this function, the callback will not be executed
     * when the Defer object goes out of scope.
     */
    void dismiss() noexcept {
        active = false;
    }

    F callback;

private:
    bool active;
};