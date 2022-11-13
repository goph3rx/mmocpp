#ifndef MMO_MMOHELPER_H
#define MMO_MMOHELPER_H

#include <google/protobuf/timestamp.pb.h>
#include <exception>
#include <string>
#include <string_view>

/**
 * Block this thread until we receive an interrupt (like SIGINT or SIGTERM)
 * signal.
 */
void mmo_wait_for_interrupt();

/**
 * Covert the string to UTF8.
 * @param source Source string.
 * @param encoding Encoding name.
 * @return Converted string.
 */
std::string mmo_to_utf8(const std::string& source, std::string_view encoding);

/**
 * Parse the datetime string into a timestamp (assumes UTC timezone).
 * @param source Source string.
 * @return Resulting timestamp.
 */
google::protobuf::Timestamp mmo_parse_timestamp(const std::string& source);

/**
 * Convert the timestamp to string.
 * @param timestamp Timestamp.
 * @return Datetime string.
 */
std::string mmo_string_timestamp(const google::protobuf::Timestamp& timestamp);

/**
 * Get current datetime.
 * @return Resulting timestamp
 */
google::protobuf::Timestamp mmo_now_timestamp();

inline bool operator>(const google::protobuf::Timestamp& left,
                      const google::protobuf::Timestamp& right)
{
    return left.seconds() > right.seconds();
}

#endif  // MMO_MMOHELPER_H
