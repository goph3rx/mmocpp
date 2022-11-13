#include <MMOWriter.h>
#include <gtest/gtest.h>
#include <asio/buffer.hpp>
#include <stdexcept>
#include <vector>

TEST(MMOWriter, skip)
{
    // Given
    std::vector<char> buffer(1, 0);
    MMOWriter writer(asio::buffer(buffer));

    // When
    writer.skip(1);

    // Then
    ASSERT_EQ(writer.size(), 1);
    ASSERT_EQ(buffer[0], 0);
}

TEST(MMOWriter, skip_overflow)
{
    // Given
    std::vector<char> buffer;
    MMOWriter writer(asio::buffer(buffer));

    // When/Then
    ASSERT_THROW(writer.skip(1), std::range_error);
}

TEST(MMOWriter, write_c)
{
    // Given
    std::vector<char> buffer(1, 0);
    MMOWriter writer(asio::buffer(buffer));

    // When
    writer.write_c(0x7B);

    // Then
    ASSERT_EQ(writer.size(), 1);
    ASSERT_EQ(buffer[0], 0x7B);
}

TEST(MMOWriter, write_c_overflow)
{
    // Given
    std::vector<char> buffer;
    MMOWriter writer(asio::buffer(buffer));

    // When/Then
    ASSERT_THROW(writer.write_c(0x7B), std::range_error);
}

TEST(MMOWriter, write_h)
{
    // Given
    std::vector<char> buffer(2, 0);
    MMOWriter writer(asio::buffer(buffer));

    // When
    writer.write_h(0x105C);

    // Then
    ASSERT_EQ(writer.size(), 2);
    ASSERT_EQ(buffer[0], 0x5C);
    ASSERT_EQ(buffer[1], 0x10);
}

TEST(MMOWriter, write_h_overflow)
{
    // Given
    std::vector<char> buffer;
    MMOWriter writer(asio::buffer(buffer));

    // When/Then
    ASSERT_THROW(writer.write_h(0x105C), std::range_error);
}

TEST(MMOWriter, write_d)
{
    // Given
    std::vector<char> buffer(4, 0);
    MMOWriter writer(asio::buffer(buffer));

    // When
    writer.write_d(0x105C6A7B);

    // Then
    ASSERT_EQ(writer.size(), 4);
    ASSERT_EQ(buffer[0], 0x7B);
    ASSERT_EQ(buffer[1], 0x6A);
    ASSERT_EQ(buffer[2], 0x5C);
    ASSERT_EQ(buffer[3], 0x10);
}

TEST(MMOWriter, write_d_overflow)
{
    // Given
    std::vector<char> buffer;
    MMOWriter writer(asio::buffer(buffer));

    // When/Then
    ASSERT_THROW(writer.write_d(0x105C6A7B), std::range_error);
}

TEST(MMOWriter, write_b)
{
    // Given
    std::vector<char> buffer(3, 0);
    MMOWriter writer(asio::buffer(buffer));

    // When
    writer.write_b(std::string{1, 2, 3});

    // Then
    ASSERT_EQ(writer.size(), 3);
    ASSERT_EQ(buffer[0], 1);
    ASSERT_EQ(buffer[1], 2);
    ASSERT_EQ(buffer[2], 3);
}

TEST(MMOWriter, write_b_overflow)
{
    // Given
    std::vector<char> buffer;
    MMOWriter writer(asio::buffer(buffer));

    // When/Then
    ASSERT_THROW(writer.write_b(std::string{1, 2, 3}), std::range_error);
}
