#include <catch2/catch_test_macros.hpp>
#include "../core/chunk_manager.hpp"
#include "../core/did_crypto_generator.hpp"

using namespace oasis;

TEST_CASE("Story 1.1: Strict 32-bit Voxel Memory Layout", "[domain][memory]") {
    // Assert 4 byte size is maintained across platforms
    REQUIRE(sizeof(Voxel) == 4);
}

TEST_CASE("Story 1.2: DOD ChunkManager Allocation", "[domain][memory]") {
    ChunkManager chunk;
    chunk.InitializeHeadless();
    
    // Bounds checking
    REQUIRE_THROWS_AS(chunk.GetIndex(-1, 0, 0), std::out_of_range);
    REQUIRE_THROWS_AS(chunk.GetIndex(32, 0, 0), std::out_of_range);

    // Voxel mutation validation
    Voxel test_voxel{15, 255, 100, 0b10101010};
    chunk.SetVoxel(16, 16, 16, test_voxel);
    
    Voxel retrieved = chunk.GetVoxel(16, 16, 16);
    REQUIRE(retrieved.material_id == 15);
    REQUIRE(retrieved.temperature == 100);
}

TEST_CASE("Story 1.3: DID Crypto Generator", "[domain][crypto]") {
    REQUIRE(DIDCryptoGenerator::Initialize() == true);
    
    std::string did1 = DIDCryptoGenerator::GenerateDIDKey();
    std::string did2 = DIDCryptoGenerator::GenerateDIDKey();
    
    // Ensure format matches
    REQUIRE(did1.find("did:key:zHex") == 0);
    // Ensure randomness/uniqueness between runs
    REQUIRE(did1 != did2);
}
