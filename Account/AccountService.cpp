#include <Account.pb.h>
#include <AccountService.h>
#include <IAccountDatabase.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <spdlog/spdlog.h>
#include <algorithm>
#include <stdexcept>

namespace
{
/**
 * Size of the randomly generated salt.
 */
constexpr int SALT_SIZE = 16;

/**
 * Decode input with base64.
 * @param input Input string.
 * @return Decoded buffer.
 */
std::string base64_decode(const std::string& input)
{
    std::string result(input.size(), 0);
    auto size =
        EVP_DecodeBlock(reinterpret_cast<unsigned char*>(result.data()),
                        reinterpret_cast<const unsigned char*>(input.data()),
                        static_cast<int>(input.size()));
    if (size == -1)
    {
        throw std::runtime_error("Failed to decode base64 string");
    }
    result.resize(size);
    return result;
}

/**
 * Encode input with base64.
 * @param buffer Input buffer.
 * @param size Size of input.
 * @return Encoded string.
 */
std::string base64_encode(unsigned char* buffer, int size)
{
    std::string result(static_cast<std::size_t>(size) * 2, 0);
    auto encoded_size = EVP_EncodeBlock(
        reinterpret_cast<unsigned char*>(result.data()), buffer, size);
    result.resize(encoded_size);
    return result;
}

/**
 * Create a password hash.
 * @param salt Salt.
 * @param password Password in plain text.
 * @return Resulting hash.
 */
std::string hash_password(const unsigned char* salt,
                          const std::string& password)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX context;
    SHA256_Init(&context);
    SHA256_Update(&context, salt, SALT_SIZE);
    SHA256_Update(&context, password.data(), password.size());
    auto rc = SHA256_Final(hash, &context);
    if (rc != 1)
    {
        throw std::runtime_error("Failed to generate hash");
    }
    return base64_encode(hash, SHA256_DIGEST_LENGTH);
}

}

AccountService::AccountService(IAccountDatabase* database) : _database(database)
{
}

void AccountService::create(const std::string& username,
                            const std::string& password)
{
    // Generate salt
    unsigned char salt[SALT_SIZE];
    auto rc = RAND_bytes(salt, SALT_SIZE);
    if (rc != 1)
    {
        throw std::runtime_error("Failed to generate salt");
    }

    // Create the record
    AccountRecord record;
    record.mutable_username()->assign(username);
    record.mutable_salt()->assign(base64_encode(salt, SALT_SIZE));
    record.mutable_password()->assign(hash_password(salt, password));
    record.set_last_world(-1);
    _database->create(record);
}

std::optional<AccountRecord> AccountService::find(const std::string& username,
                                                  const std::string& password)
{
    SPDLOG_DEBUG("Finding account with username={}", username);

    // Load the record
    auto record = _database->get(username);
    if (!record)
    {
        SPDLOG_DEBUG("Account with username={} was not found", username);
        return record;
    }

    // Check password
    auto salt = base64_decode(record->salt());
    salt.resize(std::min(salt.size(), static_cast<std::size_t>(SALT_SIZE)));
    if (salt.size() != SALT_SIZE)
    {
        throw std::runtime_error("Invalid salt size");
    }
    auto hash = hash_password(
        reinterpret_cast<const unsigned char*>(salt.data()), password);
    if (hash.size() != record->password().size() ||
        CRYPTO_memcmp(hash.data(), record->password().data(), hash.size()) != 0)
    {
        SPDLOG_DEBUG("Account with username={} got password mismatch",
                     username);
        return {};
    }

    // Return result
    record->mutable_salt()->clear();
    record->mutable_password()->clear();
    SPDLOG_DEBUG("Account with username={} was {}", username,
                 record->ShortDebugString());
    return record;
}
