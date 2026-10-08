#include <iostream>
#include <string>
#include <cstdint>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "uhai/uhai_ring_buffer.hpp"

int main(int argc, char* argv[]) {
    std::string shm_name = oasis::uhai::DEFAULT_SHM_NAME;
    std::string action = "inspect";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--shm-name" && i + 1 < argc) {
            shm_name = argv[++i];
        } else if (arg == "--action" && i + 1 < argc) {
            action = argv[++i];
        }
    }

    if (action == "unlink") {
        bool ok = oasis::uhai::UhaiTelemetryChannel::Unlink(shm_name);
        std::cout << "UNLINK_RESULT: " << (ok ? "SUCCESS" : "FAILED") << std::endl;
        return ok ? 0 : 1;
    }

    int fd = ::shm_open(shm_name.c_str(), O_RDONLY, 0666);
    if (fd < 0) {
        std::cout << "SHM_STATUS: NOT_FOUND errno=" << errno << std::endl;
        return 2;
    }

    struct stat sb{};
    ::fstat(fd, &sb);

    void* ptr = ::mmap(nullptr, oasis::uhai::TOTAL_SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cout << "SHM_STATUS: MMAP_FAILED errno=" << errno << std::endl;
        ::close(fd);
        return 3;
    }

    auto* hdr = reinterpret_cast<const oasis::uhai::SharedRingHeader*>(ptr);

    std::cout << "HEADER_INFO: size=" << sb.st_size
              << " magic=0x" << std::hex << hdr->magic_signature << std::dec
              << " version=" << hdr->version
              << " capacity=" << hdr->capacity
              << " element_size=" << hdr->element_size
              << " write_index=" << hdr->write_index.load()
              << " read_index=" << hdr->read_index.load()
              << " dropped_frames=" << hdr->dropped_frames.load()
              << std::endl;

    ::munmap(ptr, oasis::uhai::TOTAL_SHM_SIZE);
    ::close(fd);
    return 0;
}
