#ifndef MMO_IACCOUNTSERVICE_H
#define MMO_IACCOUNTSERVICE_H

#include <Account.pb.h>
#include <optional>
#include <string>

/**
 * Interface for the service that manages accounts.
 */
class IAccountService
{
   public:
    virtual ~IAccountService() = default;

    /**
     * Create a new account.
     * @param username Account username.
     * @param password Account password.
     */
    virtual void create(const std::string& username,
                        const std::string& password) = 0;

    /**
     * Fetch the account.
     * @param username Account username.
     * @param password Account password.
     * @return Account, if found.
     */
    virtual std::optional<AccountRecord> find(const std::string& username,
                                              const std::string& password) = 0;
};

#endif  // MMO_IACCOUNTSERVICE_H
