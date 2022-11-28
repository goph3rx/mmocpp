#ifndef MMO_TRANSFERDATABASE_H
#define MMO_TRANSFERDATABASE_H

#include <ITransferDatabase.h>
#include <string>

/**
 * Database adapter for storing transfers.
 */
class TransferDatabase : public ITransferDatabase
{
   public:
    /**
     * Create a new database adapter.
     */
    explicit TransferDatabase(std::string options);

    void ping() override;
    void create(const TransferRecord& record) override;

    TransferDatabase(const TransferDatabase&) = delete;
    TransferDatabase& operator=(const TransferDatabase&) = delete;

   private:
    /**
     * Options for creating a connection.
     */
    std::string _options;
};

#endif  // MMO_TRANSFERDATABASE_H
