
#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

/**
 * A simple arena allocator that allocates memory in chunks.
 * It is designed for fast allocation of many small objects.
 * The allocated memory is freed when the arena is destroyed.
 * The arena does not support deallocation of individual objects.
 * All objects must be trivially destructible.
 *
 * NOTE: this code is unfinished and may be incomplete or incorrect. Use at your own risk.
 */
class Arena {
public:
    explicit Arena(size_t chunk_size = 64 * 1024)
        : chunk_size_(chunk_size) {
        assert(chunk_size > 0);
    }

    ~Arena() {
        Chunk* chunk = head_;
        while (chunk) {
            Chunk* next = chunk->next;
            ::operator delete(chunk);
            chunk = next;
        }
    }

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    template<typename T, typename... Args>
    T* alloc(Args&&... args) {
        static_assert(std::is_trivially_destructible_v<T>,
                      "Arena only supports trivially destructible types");

        void* memory = alloc_raw(sizeof(T), alignof(T));
        return new (memory) T{std::forward<Args>(args)...};
    }

    template<typename T>
    T* alloc_array(size_t count) {
        static_assert(std::is_trivially_destructible_v<T>,
                      "Arena only supports trivially destructible types");

        if (count > std::numeric_limits<size_t>::max() / sizeof(T))
            throw std::bad_array_new_length{};

        size_t size = sizeof(T) * count;
        void* memory = alloc_raw(size, alignof(T));

        T* result = static_cast<T*>(memory);

        for (size_t i = 0; i < count; ++i)
            new (&result[i]) T{};

        return result;
    }

    void* alloc_raw(size_t size, size_t alignment) {
        assert(alignment != 0);
        assert((alignment & (alignment - 1)) == 0);

        if (void* memory = try_alloc(size, alignment))
            return memory;

        size_t required = size + alignment - 1;
        if (required < size)
            throw std::bad_alloc{};

        size_t capacity = chunk_size_ > required
                        ? chunk_size_
                        : required;

        add_chunk(capacity);

        void* memory = try_alloc(size, alignment);
        assert(memory != nullptr);
        return memory;
    }

    void reset() {
        current_ = head_;

        for (Chunk* chunk = head_; chunk; chunk = chunk->next)
            chunk->used = 0;
    }

    size_t used() const {
        size_t total = 0;
        for (Chunk* chunk = head_; chunk; chunk = chunk->next)
            total += chunk->used;
        return total;
    }

    size_t capacity() const {
        size_t total = 0;
        for (Chunk* chunk = head_; chunk; chunk = chunk->next)
            total += chunk->capacity;
        return total;
    }

private:
    struct Chunk {
        Chunk* next;
        size_t capacity;
        size_t used;
    };

    Chunk* head_ = nullptr;
    Chunk* current_ = nullptr;
    size_t chunk_size_;

    void add_chunk(size_t capacity) {
        if (capacity > std::numeric_limits<size_t>::max()
                       - sizeof(Chunk))
            throw std::bad_alloc{};

        void* memory = ::operator new(sizeof(Chunk) + capacity);
        auto* chunk = new (memory) Chunk{
            nullptr, capacity, 0
        };

        if (!head_) {
            head_ = chunk;
        } else {
            Chunk* tail = head_;
            while (tail->next)
                tail = tail->next;
            tail->next = chunk;
        }

        current_ = chunk;
    }

    void* try_alloc(size_t size, size_t alignment) {
        while (current_) {
            uintptr_t base = reinterpret_cast<uintptr_t>(current_ + 1);
            uintptr_t ptr = base + current_->used;

            uintptr_t aligned =
                (ptr + alignment - 1) & ~(alignment - 1);

            size_t padding = aligned - ptr;

            if (padding <= current_->capacity - current_->used &&
                size <= current_->capacity - current_->used - padding) {

                current_->used += padding + size;
                return reinterpret_cast<void*>(aligned);
            }

            current_ = current_->next;
        }

        return nullptr;
    }
};
