#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdint>
#include <cstdlib>
#include "uhai/uhai_ring_buffer.hpp"

int main(int argc, char* argv[]) {
    std::string shm_name = oasis::uhai::DEFAULT_SHM_NAME;
    uint64_t total_frames = 1000;
    uint32_t delay_us = 0;
    bool retry_on_full = false;
    bool unlink_before = false;
    bool unlink_after = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--shm-name" && i + 1 < argc) {
            shm_name = argv[++i];
        } else if (arg == "--frames" && i + 1 < argc) {
            total_frames = std::stoull(argv[++i]);
        } else if (arg == "--delay-us" && i + 1 < argc) {
            delay_us = static_cast<uint32_t>(std::stoul(argv[++i]));
        } else if (arg == "--retry-on-full") {
            retry_on_full = true;
        } else if (arg == "--unlink-before") {
            unlink_before = true;
        } else if (arg == "--unlink-after") {
            unlink_after = true;
        }
    }

    if (unlink_before) {
        oasis::uhai::UhaiTelemetryChannel::Unlink(shm_name);
    }

    oasis::uhai::UhaiTelemetryChannel producer(oasis::uhai::UhaiTelemetryChannel::ChannelMode::Producer, shm_name);
    if (!producer.IsValid()) {
        std::cerr << "Failed to initialize producer on " << shm_name << "\n";
        return 1;
    }

    uint64_t accepted = 0;
    uint64_t rejected = 0;

    for (uint64_t i = 0; i < total_frames; ++i) {
        oasis::uhai::TelemetrySample sample{};
        sample.frame_count = i;
        sample.timestamp_ns = 1'000'000'000ULL + i * 16'666'666ULL;
        sample.fps = 60.0f;
        sample.founder_stress = 0.25f + static_cast<float>(i % 10) * 0.05f;
        sample.memory_slot_count = static_cast<uint32_t>(i % 32);
        sample.biome_id = static_cast<uint8_t>(i % 3);
        sample.quality_flags = 0x01;
        sample.reserved = 0;

        if (retry_on_full) {
            while (!producer.Push(sample)) {
                rejected++;
                std::this_thread::yield();
            }
            accepted++;
        } else {
            if (producer.Push(sample)) {
                accepted++;
            } else {
                rejected++;
            }
        }

        if (delay_us > 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(delay_us));
        }
    }

    std::cout << "PRODUCER_SUMMARY: accepted=" << accepted
              << " rejected=" << rejected
              << " dropped_in_header=" << producer.GetHeader()->dropped_frames.load()
              << std::endl;

    if (unlink_after) {
        oasis::uhai::UhaiTelemetryChannel::Unlink(shm_name);
    }

    return 0;
}
