#include "storageunit.h"

StorageUnit::StorageUnit(int newStatus) : modifyStatus(newStatus)
{
}

int StorageUnit::getModifyStatus()
{
    return modifyStatus;
}

void StorageUnit::setModifyStatus(int m)
{
    modifyStatus = m;
}
