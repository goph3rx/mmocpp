#ifndef MMO_ACCOUNTDATABASEMOCK_H
#define MMO_ACCOUNTDATABASEMOCK_H

#include <IAccountDatabase.h>
#include <gmock/gmock.h>

/**
 * Mock database for accounts.
 */
class AccountDatabaseMock final : public IAccountDatabase
{
   public:
    MOCK_METHOD(void, ping, (), (override));
    MOCK_METHOD(void, create, (const AccountRecord&), (override));
    MOCK_METHOD(std::optional<AccountRecord>,
                get,
                (const std::string& username),
                (override));
};

#endif  // MMO_ACCOUNTDATABASEMOCK_H
