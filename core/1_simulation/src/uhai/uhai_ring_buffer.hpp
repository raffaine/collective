#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace uhai {

// Basic 24-byte TelemetrySample struct
struct TelemetrySample {
    uint64_t timestamp; // 8 bytes
    double position[2]; // 16 bytes
};

// SPSC (Single-Producer Single-Consumer) Lock-Free Ring Buffer
template <typename T, size_t Capacity>
class SPSCRingBuffer {
public:
    SPSCRingBuffer() : read_index_(0), write_index_(0) {}

    // Disable copy and move semantics to prevent accidental sharing/copying
    SPSCRingBuffer(const SPSCRingBuffer&) = delete;
    SPSCRingBuffer& operator=(const SPSCRingBuffer&) = delete;

    // Push an item into the buffer
    bool push(const T& item) {
        const size_t current_write = write_index_.load(std::memory_order_relaxed);
        const size_t next_write = increment(current_write);
        
        // Acquire barrier ensures all earlier writes to buffer_ in pop are visible if we read an updated read_index_
        if (next_write == read_index_.load(std::memory_order_acquire)) {
            // Buffer is full
            return false;
        }

        buffer_[current_write] = item;
        // Release barrier ensures the write to buffer_ is visible before write_index_ update
        write_index_.store(next_write, std::memory_order_release);
        return true;
    }

    // Pop an item from the buffer
    std::optional<T> pop() {
        const size_t current_read = read_index_.load(std::memory_order_relaxed);
        
        // Acquire barrier ensures all earlier writes to buffer_ in push are visible if we read an updated write_index_
        if (current_read == write_index_.load(std::memory_order_acquire)) {
            // Buffer is empty
            return std::nullopt;
        }

        T item = buffer_[current_read];
        // Release barrier ensures the read from buffer_ is complete before read_index_ update
        read_index_.store(increment(current_read), std::memory_order_release);
        return item;
    }

private:
    static constexpr size_t increment(size_t index) {
        return (index + 1) % Capacity;
    }

    // alignas(64) ensures strict 64-byte cache-line padding between the atomic read index, 
    // the atomic write index, and the payload buffer, to completely eliminate false sharing.
    alignas(64) std::atomic<size_t> read_index_;
    alignas(64) std::atomic<size_t> write_index_;
    alignas(64) T buffer_[Capacity];
};

} // namespace uhai
