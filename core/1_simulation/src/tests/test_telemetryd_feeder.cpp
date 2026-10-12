#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include "uhai/uhai_ring_buffer.hpp"

int main() {
    const char* shm_name = "/test_col_telemetry_shm";
    ::shm_unlink(shm_name);

    int fd = ::shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        std::cerr << "Failed to shm_open: " << strerror(errno) << std::endl;
        return 1;
    }

    if (::ftruncate(fd, oasis::uhai::TOTAL_SHM_SIZE) != 0) {
        std::cerr << "Failed to ftruncate: " << strerror(errno) << std::endl;
        return 1;
    }

    void* raw_ptr = ::mmap(nullptr, oasis::uhai::TOTAL_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (raw_ptr == MAP_FAILED) {
        std::cerr << "Failed to mmap: " << strerror(errno) << std::endl;
        return 1;
    }

    auto* header = reinterpret_cast<oasis::uhai::SharedRingHeader*>(raw_ptr);
    header->write_index.store(0);
    header->read_index.store(0);
    header->dropped_frames.store(0);
    header->capacity = oasis::uhai::RING_CAPACITY;
    header->element_size = sizeof(oasis::uhai::TelemetrySlot);
    header->magic_signature = oasis::uhai::UHAI_MAGIC;
    header->version = oasis::uhai::UHAI_VERSION;

    oasis::uhai::LockFreeSpscRing<oasis::uhai::TelemetrySlot, oasis::uhai::RING_CAPACITY> ring(raw_ptr);

    // Frame 1: QUALITY_SIMULATED (0x01) -> MUST BE DROPPED AS SECURITY VIOLATION
    oasis::uhai::TelemetrySample s1{};
    s1.frame_count = 1;
    s1.quality_flags = oasis::uhai::QUALITY_SIMULATED;
    s1.fps = 60.0f;
    ring.Push(s1);

    // Frame 2: QUALITY_HARDWARE_ROOT (0x02) -> MUST BE ACCEPTED FOR PHYSICAL HAL
    oasis::uhai::TelemetrySample s2{};
    s2.frame_count = 2;
    s2.quality_flags = oasis::uhai::QUALITY_HARDWARE_ROOT;
    s2.fps = 60.0f;
    ring.Push(s2);

    // Frame 3: QUALITY_SIMULATED | QUALITY_HARDWARE_ROOT (0x03) -> MUST BE DROPPED AS INVALID
    oasis::uhai::TelemetrySample s3{};
    s3.frame_count = 3;
    s3.quality_flags = 0x03;
    s3.fps = 60.0f;
    ring.Push(s3);

    std::cout << "Successfully pushed 3 test frames to " << shm_name << std::endl;

    ::munmap(raw_ptr, oasis::uhai::TOTAL_SHM_SIZE);
    ::close(fd);
    return 0;
}
