#include <sodium.h>

#include "crypto_utils.hpp"

#include <stdexcept>

namespace bc
{

    // Initialize libsodium
    static bool sodium_ready = (sodium_init() >= 0);

    //
    // Hex Helpers
    //

    std::string
    to_hex(const unsigned char* data, std::size_t len)
    {
        std::string out;
        out.resize(len * 2);

        sodium_bin2hex(
            out.data(),
            out.size() + 1,
            data,
            len
        );

        return out;
    }

    std::vector<unsigned char>
    from_hex(const std::string& hex)
    {
        if (hex.size() % 2 != 0)
        {
            throw std::invalid_argument("Invalid hex encoding.");
        }

        std::vector<unsigned char> out(hex.size() / 2);

        std::size_t bin_len = 0;

        if (sodium_hex2bin(
                out.data(),
                out.size(),
                hex.data(),
                hex.size(),
                nullptr,
                &bin_len,
                nullptr
            ) != 0)
        {
            throw std::invalid_argument("Invalid hex encoding.");
        }

        out.resize(bin_len);
        return out;
    }

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

        return to_hex(hash, crypto_hash_sha256_BYTES);
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

        return to_hex(hash, crypto_generichash_BYTES);
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
        // Reject signatures that don't match Ed25519 signature length
        if (signature.size() != crypto_sign_BYTES)
            return false;

        return crypto_sign_verify_detached(
            signature.data(),
            reinterpret_cast<const unsigned char*>(message.data()),
            message.size(),
            pk.data()
        ) == 0;
    }

} // namespace bc
