#include "AccountDatabaseMock.h"

#include <Account.pb.h>
#include <AccountService.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <optional>
#include <stdexcept>

using namespace ::testing;

TEST(AccountService, create_failed)
{
    // Given
    AccountDatabaseMock database;
    AccountService service(&database);
    EXPECT_CALL(database, create(_))
        .WillOnce(Throw(std::runtime_error("Database failed")));

    // When/Then
    ASSERT_THROW(service.create("hello", "world"), std::runtime_error);
}

TEST(AccountService, create_success)
{
    // Given
    AccountDatabaseMock database;
    AccountService service(&database);
    auto is_valid = [](const AccountRecord& record)
    {
        return record.username() == "hello" && !record.salt().empty() &&
               !record.password().empty() && record.last_world() == -1 &&
               !record.has_banned_until();
    };
    EXPECT_CALL(database, create(Truly(is_valid))).WillOnce(Return());

    // When/Then
    service.create("hello", "world");
}

TEST(AccountService, find_failed)
{
    // Given
    AccountDatabaseMock database;
    AccountService service(&database);
    EXPECT_CALL(database, get("hello"))
        .WillOnce(Throw(std::runtime_error("Database failed")));

    // When/Then
    ASSERT_THROW(service.find("hello", "test"), std::runtime_error);
}

TEST(AccountService, find_not_found)
{
    // Given
    AccountDatabaseMock database;
    AccountService service(&database);
    EXPECT_CALL(database, get("hello"))
        .WillOnce(Return(std::optional<AccountRecord>()));

    // When
    auto account = service.find("hello", "test");

    // Then
    ASSERT_FALSE(account);
}

TEST(AccountService, find_invalid_salt)
{
    // Given
    AccountDatabaseMock database;
    AccountService service(&database);
    AccountRecord record;
    record.set_username("hello");
    record.set_salt("Cg==");
    record.set_password("bVZB2VS+pWRLiah1UDSM1/h6nJqQxgZMoMWBb+H62S4=");
    record.set_last_world(1);
    EXPECT_CALL(database, get("hello")).WillOnce(Return(record));

    // When/Then
    ASSERT_THROW(service.find("hello", "test"), std::runtime_error);
}

TEST(AccountService, find_invalid_base64)
{
    // Given
    AccountDatabaseMock database;
    AccountService service(&database);
    AccountRecord record;
    record.set_username("hello");
    record.set_salt("not-base64");
    record.set_last_world(1);
    EXPECT_CALL(database, get("hello")).WillOnce(Return(record));

    // When/Then
    ASSERT_THROW(service.find("hello", "test"), std::runtime_error);
}

TEST(AccountService, find_invalid_password)
{
    // Given
    AccountDatabaseMock database;
    AccountService service(&database);
    AccountRecord record;
    record.set_username("hello");
    record.set_salt("CHhHc9xDl5Q7c9lP5IHhMQ==");
    record.set_password("bVZB2VS+pWRLiah1UDSM1/h6nJqQxgZMoMWBb+H62S4=");
    record.set_last_world(1);
    EXPECT_CALL(database, get("hello")).WillOnce(Return(record));

    // When
    auto account = service.find("hello", "world");

    // Then
    ASSERT_FALSE(account);
}

TEST(AccountService, find_success)
{
    // Given
    AccountDatabaseMock database;
    AccountService service(&database);
    AccountRecord record;
    record.set_username("hello");
    record.set_salt("CHhHc9xDl5Q7c9lP5IHhMQ==");
    record.set_password("bVZB2VS+pWRLiah1UDSM1/h6nJqQxgZMoMWBb+H62S4=");
    record.set_last_world(1);
    EXPECT_CALL(database, get("hello")).WillOnce(Return(record));

    // When
    auto account = service.find("hello", "test");

    // Then
    ASSERT_TRUE(account);
    ASSERT_STREQ(account->username().c_str(), "hello");
    ASSERT_TRUE(account->salt().empty());
    ASSERT_TRUE(account->password().empty());
    ASSERT_EQ(account->last_world(), 1);
    ASSERT_FALSE(account->has_banned_until());
}
