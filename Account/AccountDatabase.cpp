#include <AccountDatabase.h>
#include <MMOHelper.h>
#include <spdlog/spdlog.h>
#include <cstdint>
#include <exception>
#include <pqxx/pqxx>
#include <utility>

namespace
{
/**
 * Connect to the database.
 * @param options Options for creating the connections.
 * @return Resulting connection.
 */
pqxx::connection connect(const std::string& options)
{
    pqxx::connection connection(options);
    connection.set_client_encoding("UTF8");
    return connection;
}

}

AccountDatabase::AccountDatabase(std::string options)
    : _options(std::move(options))
{
}

void AccountDatabase::ping()
{
    try
    {
        auto connection = connect(_options);
    }
    catch (const std::exception& e)
    {
        SPDLOG_WARN("Failed to ping: {}", e.what());
    }
}

void AccountDatabase::create(const AccountRecord& record)
{
    SPDLOG_DEBUG("Creating new account with username={}", record.username());

    auto connection = connect(_options);
    connection.prepare("account_create",
                       "INSERT INTO accounts (username, salt, password, "
                       "last_world) VALUES ($1, $2, $3, $4)");
    pqxx::work transaction(connection);
    transaction.exec_prepared("account_create", record.username(),
                              record.salt(), record.password(),
                              record.last_world());
    transaction.commit();

    SPDLOG_DEBUG("Success");
}

std::optional<AccountRecord> AccountDatabase::get(const std::string& username)
{
    SPDLOG_DEBUG("Getting account with username={}", username);

    // Query the database
    auto connection = connect(_options);
    connection.prepare("account_get",
                       "SELECT salt, password, last_world, banned_until FROM "
                       "accounts WHERE username = $1 LIMIT 1");
    pqxx::work transaction(connection);
    auto result = transaction.exec_prepared("account_get", username);
    transaction.commit();
    if (result.empty())
    {
        SPDLOG_DEBUG("Account with username={} was not found", username);
        return {};
    }

    // Extract the result
    auto row = result.at(0);
    AccountRecord record;
    record.mutable_username()->assign(username);
    record.mutable_salt()->assign(row["salt"].as<std::string>());
    record.mutable_password()->assign(row["password"].as<std::string>());
    record.set_last_world(row["last_world"].as<int32_t>());
    auto banned_util = row["banned_until"];
    if (!banned_util.is_null())
    {
        *record.mutable_banned_until() =
            mmo_parse_timestamp(banned_util.as<std::string>());
    }
    SPDLOG_DEBUG("Account with username={} was found", username);
    return record;
}