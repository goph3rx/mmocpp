#ifndef MMO_ACCOUNTSERVICE_H
#define MMO_ACCOUNTSERVICE_H

#include <IAccountService.h>

class IAccountDatabase;

/**
 * Service for managing accounts.
 */
class AccountService final : public IAccountService
{
   public:
    /**
     * Create a new service.
     * @param database Database for storing accounts.
     */
    explicit AccountService(IAccountDatabase* database);

    AccountService(const AccountService&) = delete;
    AccountService& operator=(const AccountService&) = delete;

    void create(const std::string& username,
                const std::string& password) override;
    std::optional<AccountRecord> find(const std::string& username,
                                      const std::string& password) override;

   private:
    /**
     * Database for storing accounts.
     */
    IAccountDatabase* _database;
};

#endif  // MMO_ACCOUNTSERVICE_H
