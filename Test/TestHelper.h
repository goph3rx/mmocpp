#ifndef MMO_TESTHELPER_H
#define MMO_TESTHELPER_H

#include <asio/buffer.hpp>
#include <string>

/**
 * Create a string from hex.
 * @param hex Hexadecimal representation.
 * @return Decoded string.
 */
std::string test_from_hex(const std::string& hex);

/**
 * Convert a string to hex.
 * @param s Source.
 * @return Hexadecimal representation.
 */
std::string test_to_hex(asio::const_buffer s);

#endif  // MMO_TESTHELPER_H
