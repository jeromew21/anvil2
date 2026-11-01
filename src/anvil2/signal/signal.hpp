#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

/**
 * A thread-safe signal/slot implementation.
 * Allows connecting, disconnecting, and emitting signals with arbitrary arguments.
 */
template<typename... Args>
class Signal {
public:
    using Callback = std::function<void(Args...)>;
    using Id = std::uint64_t;

    Id connect(Callback cb) {
        Id id;
        std::shared_ptr<const std::vector<Slot>> old;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto next = std::make_shared<std::vector<Slot>>(*slots_);
            id = next_id_++;
            next->push_back({id, std::move(cb)});
            old = std::move(slots_);
            slots_ = std::move(next);
        }
        // `old` is released here, outside the lock
        return id;
    }

    void disconnect(Id id) {
        std::shared_ptr<const std::vector<Slot>> old;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto next = std::make_shared<std::vector<Slot>>(*slots_);
            next->erase(
                std::remove_if(next->begin(), next->end(),
                               [id](const Slot& s) { return s.id == id; }),
                next->end());
            old = std::move(slots_);
            slots_ = std::move(next);
        }
    }

    void emit(Args... args) const {
        std::shared_ptr<const std::vector<Slot>> snapshot;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            snapshot = slots_;
        }
        for (const auto& slot : *snapshot) {
            slot.callback(args...);
        }
    }

private:
    struct Slot {
        Id id;
        Callback callback;
    };

    mutable std::mutex mutex_;
    std::shared_ptr<const std::vector<Slot>> slots_ =
        std::make_shared<std::vector<Slot>>();
    Id next_id_ = 0;
};