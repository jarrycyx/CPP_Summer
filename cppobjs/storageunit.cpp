#include "storageunit.h"

StorageUnit::StorageUnit(ModifyStatus newStatus) : modifyStatus(newStatus)
{
}

int StorageUnit::getModifyStatus()
{
    return modifyStatus;
}

void StorageUnit::setModifyStatus(ModifyStatus m)
{
    modifyStatus = m;
}
