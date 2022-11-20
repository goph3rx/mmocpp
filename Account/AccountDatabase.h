#ifndef MMO_ACCOUNTDATABASE_H
#define MMO_ACCOUNTDATABASE_H

#include <IAccountDatabase.h>
#include <string>

/**
 * Database adapter for storing accounts.
 */
class AccountDatabase final : public IAccountDatabase
{
   public:
    /**
     * Create a new database adapter.
     */
    explicit AccountDatabase(std::string options);
    AccountDatabase(const AccountDatabase&) = delete;
    AccountDatabase& operator=(const AccountDatabase&) = delete;

    void ping() override;
    void create(const AccountRecord& record) override;
    std::optional<AccountRecord> get(const std::string& username) override;

   private:
    /**
     * Options for creating a connection.
     */
    std::string _options;
};

#endif  // MMO_ACCOUNTDATABASE_H
