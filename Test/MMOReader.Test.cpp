#include <MMOReader.h>
#include <gtest/gtest.h>
#include <asio/buffer.hpp>
#include <stdexcept>
#include <vector>
#include "TestHelper.h"

TEST(MMOReader, skip)
{
    // Given
    std::vector<char> buffer(1, 0);
    MMOReader reader(asio::buffer(buffer));

    // When
    reader.skip(1);

    // Then
    ASSERT_EQ(reader.position(), 1);
}

TEST(MMOReader, skip_overflow)
{
    // Given
    std::vector<char> buffer;
    MMOReader reader(asio::buffer(buffer));

    // When/Then
    ASSERT_THROW(reader.skip(1), std::range_error);
}

TEST(MMOReader, read_b)
{
    // Given
    const char buffer[] = "hello";
    MMOReader reader(asio::buffer(buffer));

    // When
    auto value = reader.read_b(5);

    // Then
    ASSERT_EQ(reader.position(), 5);
    ASSERT_STREQ(value.c_str(), "hello");
}

TEST(MMOReader, read_b_overflow)
{
    // Given
    std::vector<char> buffer;
    MMOReader reader(asio::buffer(buffer));

    // When/Then
    ASSERT_THROW(reader.read_b(5), std::range_error);
}

TEST(MMOReader, read_c)
{
    // Given
    auto buffer = test_from_hex("10");
    MMOReader reader(asio::buffer(buffer));

    // When
    auto value = reader.read_c();

    // Then
    ASSERT_EQ(reader.position(), 1);
    ASSERT_EQ(value, 0x10);
}

TEST(MMOReader, read_c_overflow)
{
    // Given
    std::vector<char> buffer;
    MMOReader reader(asio::buffer(buffer));

    // When/Then
    ASSERT_THROW(reader.read_c(), std::range_error);
}

TEST(MMOReader, read_h)
{
    // Given
    auto buffer = test_from_hex("5C10");
    MMOReader reader(asio::buffer(buffer));

    // When
    auto value = reader.read_h();

    // Then
    ASSERT_EQ(reader.position(), 2);
    ASSERT_EQ(value, 0x105C);
}

TEST(MMOReader, read_h_overflow)
{
    // Given
    std::vector<char> buffer;
    MMOReader reader(asio::buffer(buffer));

    // When/Then
    ASSERT_THROW(reader.read_h(), std::range_error);
}

TEST(MMOReader, read_d)
{
    // Given
    auto buffer = test_from_hex("7B6A5C10");
    MMOReader reader(asio::buffer(buffer));

    // When
    auto value = reader.read_d();

    // Then
    ASSERT_EQ(reader.position(), 4);
    ASSERT_EQ(value, 0x105C6A7B);
}

TEST(MMOReader, read_d_overflow)
{
    // Given
    std::vector<char> buffer;
    MMOReader reader(asio::buffer(buffer));

    // When/Then
    ASSERT_THROW(reader.read_d(), std::range_error);
}

TEST(MMOReader, read_q)
{
    // Given
    auto buffer = test_from_hex("7A6B5D1F7B6A5C10");
    MMOReader reader(asio::buffer(buffer));

    // When
    auto value = reader.read_q();

    // Then
    ASSERT_EQ(reader.position(), 8);
    ASSERT_EQ(value, 0x105C6A7B1F5D6B7ALL);
}

TEST(MMOReader, read_q_overflow)
{
    // Given
    std::vector<char> buffer;
    MMOReader reader(asio::buffer(buffer));

    // When/Then
    ASSERT_THROW(reader.read_q(), std::range_error);
}
