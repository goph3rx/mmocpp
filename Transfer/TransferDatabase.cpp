#include <MMOHelper.h>
#include <TransferDatabase.h>
#include <spdlog/spdlog.h>
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

TransferDatabase::TransferDatabase(std::string options)
    : _options(std::move(options))
{
}

void TransferDatabase::ping()
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

void TransferDatabase::create(const TransferRecord& transfer)
{
    SPDLOG_DEBUG("Creating new transfer {}", transfer.ShortDebugString());

    auto connection = connect(_options);
    connection.prepare("transfer_create",
                       "INSERT INTO transfers (account, auth, play, "
                       "expiry) VALUES ($1, $2, $3, $4) ON CONFLICT "
                       "(account) DO UPDATE SET auth = EXCLUDED.auth,"
                       "play = EXCLUDED.play, expiry = EXCLUDED.expiry");

    pqxx::work transaction(connection);
    transaction.exec_prepared("transfer_create", transfer.account(),
                              transfer.auth(), transfer.play(),
                              mmo_string_timestamp(transfer.expiry()));
    transaction.commit();

    SPDLOG_DEBUG("Success");
}