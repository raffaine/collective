#include "did_crypto_generator.hpp"
#include <iostream>

#ifndef __EMSCRIPTEN__
#include <sodium.h>
#endif

namespace oasis {

bool DIDCryptoGenerator::Initialize() {
#ifndef __EMSCRIPTEN__
    if (sodium_init() < 0) {
        std::cerr << "libsodium failed to initialize, it is not safe to use." << std::endl;
        return false;
    }
    return true;
#else
    // In the browser, we will eventually bind this to the native WebCrypto API via JS.
    // For Phase 1 WASM testing, we safely mock the initialization.
    std::cout << "WASM Environment Detected: Bypassing libsodium for native WebCrypto API mock." << std::endl;
    return true;
#endif
}

std::string DIDCryptoGenerator::GenerateDIDKey() {
#ifndef __EMSCRIPTEN__
    unsigned char pk[crypto_sign_PUBLICKEYBYTES];
    unsigned char sk[crypto_sign_SECRETKEYBYTES];

    crypto_sign_keypair(pk, sk);

    // Convert public key to hex
    char hex_pk[crypto_sign_PUBLICKEYBYTES * 2 + 1];
    sodium_bin2hex(hex_pk, sizeof(hex_pk), pk, sizeof(pk));

    return std::string("did:key:zHex") + hex_pk;
#else
    // Mock DID for the browser until we write the EM_ASM JS bindings
    return std::string("did:key:zWasmMockBrowserKey99999");
#endif
}

} // namespace oasis
