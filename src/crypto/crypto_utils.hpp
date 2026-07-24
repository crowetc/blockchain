#ifndef CRYPTO_UTILS_HPP_
#define CRYPTO_UTILS_HPP_

#include <array>
#include <string>
#include <vector>

namespace bc
{
    //
    // Hashing
    //

    /** Compute SHA-256 and return a hex-encoded string (64 chars)
     *  @param data input data to hash
     *  @return Hex-encoded string
     */
    std::string
    sha256_hex(const std::string& data);

    /** Compute BLAKE2b and return a hex-encoded string (128 chars)
     *  @param data input data to hash
     *  @return Hex-encoded string
     */
    std::string
    blake2b_hex(const std::string& data);

    //
    // Key Generation
    //

    /** Generate an Ed25519 keypair.
     *  @param pk: 32-byte public key
     *  @param sk: 64-byte secret key
     */
    void
    generate_keypair(std::array<unsigned char, 32>& pk,
                     std::array<unsigned char, 64>& sk);

    //
    // Signing
    //

    /** Create an Ed25519 signature for the given message.
     *  @param message the message to sign
     *  @param sk the secret key to sign with
     *  @return detached signature (64 bytes)
     */
    std::vector<unsigned char>
    sign(const std::string& message,
         const std::array<unsigned char, 64>& sk);

    //
    // Verification
    //

    /** Verify an Ed25519 signature.
     *  @param message the signed message
     *  @param signature the signature to verify
     *  @param pk the public key for verification
     *  @return @c true if valid
     */
    bool
    verify(const std::string& message,
           const std::vector<unsigned char>& signature,
           const std::array<unsigned char, 32>& pk);

} // namespace bc

#endif // CRYPTO_UTILS_HPP_