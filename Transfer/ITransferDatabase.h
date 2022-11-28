#ifndef MMO_ITRANSFERDATABASE_H
#define MMO_ITRANSFERDATABASE_H

#include <Transfer.pb.h>

/**
 * Interface for database that holds information about transfers.
 */
class ITransferDatabase
{
   public:
    virtual ~ITransferDatabase() = default;

    /**
     * Check if the database is available.
     */
    virtual void ping() = 0;

    /**
     * Create a new transfer.
     * @param record Record to be stored in the database.
     */
    virtual void create(const TransferRecord& record) = 0;
};

#endif  // MMO_ITRANSFERDATABASE_H
