#include <sodium.h>

#include "crypto_utils.hpp"

namespace bc
{

    // Initialize libsodium
    static bool sodium_ready = (sodium_init() >= 0);

    //
    // Hashing
    //

    std::string
    sha256_hex(const std::string& data)
    {
        unsigned char hash[crypto_hash_sha256_BYTES];

        crypto_hash_sha256(
            hash,
            reinterpret_cast<const unsigned char*>(data.data()),
            data.size()
        );

        char hex[crypto_hash_sha256_BYTES * 2 + 1];
        sodium_bin2hex(hex, sizeof hex, hash, crypto_hash_sha256_BYTES);
        return std::string(hex);
    }

    std::string
    blake2b_hex(const std::string& data)
    {
        unsigned char hash[crypto_generichash_BYTES];

        crypto_generichash(
            hash, sizeof hash,
            reinterpret_cast<const unsigned char*>(data.data()),
            data.size(),
            nullptr, 0
        );

        char hex[crypto_generichash_BYTES * 2 + 1];
        sodium_bin2hex(hex, sizeof hex, hash, crypto_generichash_BYTES);
        return std::string(hex);
    }

    //
    // Key Generation
    //

    void
    generate_keypair(std::array<unsigned char, 32>& pk,
                     std::array<unsigned char, 64>& sk)
    {
        crypto_sign_keypair(pk.data(), sk.data());
    }

    //
    // Signing
    //

    std::vector<unsigned char>
    sign(const std::string& message,
         const std::array<unsigned char, 64>& sk)
    {
        std::vector<unsigned char> sig(crypto_sign_BYTES);

        crypto_sign_detached(
            sig.data(), nullptr,
            reinterpret_cast<const unsigned char*>(message.data()),
            message.size(),
            sk.data()
        );

        return sig;
    }

    //
    // Verification
    //

    bool
    verify(const std::string& message,
           const std::vector<unsigned char>& signature,
           const std::array<unsigned char, 32>& pk)
    {

        return crypto_sign_verify_detached(
            signature.data(),
            reinterpret_cast<const unsigned char*>(message.data()),
            message.size(),
            pk.data()
        ) == 0;
    }

} // namespace bc
