#ifndef MMO_IACCOUNTDATABASE_H
#define MMO_IACCOUNTDATABASE_H

#include <Account.pb.h>
#include <optional>
#include <string>

/**
 * Interface for database that holds account information.
 */
class IAccountDatabase
{
   public:
    virtual ~IAccountDatabase() = default;

    /**
     * Check if the database is available.
     */
    virtual void ping() = 0;

    /**
     * Create a new account.
     * @param record Record to be stored in the database.
     */
    virtual void create(const AccountRecord& record) = 0;

    /**
     * Fetch the account.
     * @param username Account username.
     * @return Account, if found.
     */
    virtual std::optional<AccountRecord> get(const std::string& username) = 0;
};

#endif  // MMO_IACCOUNTDATABASE_H
