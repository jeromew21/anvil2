#pragma once

#include <utility>
#include <type_traits>

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

    void dismiss() noexcept {
        active = false;
    }

    F callback;

private:
    bool active;
};