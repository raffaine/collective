#pragma once
#include <string>
#include <vector>

namespace oasis {

// Story 1.3: DID Crypto Generator using libsodium
class DIDCryptoGenerator {
public:
    static bool Initialize();
    
    // Generates a new Ed25519 keypair and returns the decentralized identifier (DID)
    // formatted as a mock did:key using hex encoding for simplicity in this crucible phase.
    static std::string GenerateDIDKey();
};

} // namespace oasis
