#include "TestHelper.h"

#include <MMOHelper.h>
#include <gmock/gmock.h>
#include <google/protobuf/timestamp.pb.h>
#include <gtest/gtest.h>
#include <string>

using namespace ::testing;

TEST(MMOHelper, to_utf8_from_latin)
{
    // Given
    std::string source("test");
    const char encoding[] = "ISO-8859-1";

    // When
    auto converted = mmo_to_utf8(source, encoding);

    // Then
    ASSERT_STREQ(converted.c_str(), "test");
}

TEST(MMOHelper, to_utf8_from_utf16)
{
    // Given
    auto source = test_from_hex("7400650073007400");
    const char encoding[] = "UTF-16LE";

    // When
    auto converted = mmo_to_utf8(source, encoding);

    // Then
    ASSERT_STREQ(converted.c_str(), "test");
}

TEST(MMOHelper, parse_timestamp_failed)
{
    // Given
    auto source = "not valid";

    // When/Then
    ASSERT_THROW(mmo_parse_timestamp(source), std::runtime_error);
}

TEST(MMOHelper, parse_timestamp_success)
{
    // Given
    auto source = "2022-10-30 10:55:15";

    // When
    auto timestamp = mmo_parse_timestamp(source);

    // Then
    ASSERT_EQ(timestamp.seconds(), 1667127315);
}

TEST(MMOHelper, string_timestamp)
{
    // Given
    google::protobuf::Timestamp timestamp;
    timestamp.set_seconds(1667609999);

    // When
    auto result = mmo_string_timestamp(timestamp);

    // Then
    ASSERT_THAT(result, StartsWith("2022-11"));
}

TEST(MMOHelper, now_timestamp)
{
    // When
    auto timestamp = mmo_now_timestamp();

    // Then
    ASSERT_GE(timestamp.seconds(), 0);
}
