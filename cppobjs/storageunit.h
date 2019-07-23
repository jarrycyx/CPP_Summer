#ifndef STORAGEUNIT_H
#define STORAGEUNIT_H

class StorageUnit
{
public:
    StorageUnit(int newStatus);
    int getModifyStatus();
    void setModifyStatus(int m);

private:
    int modifyStatus; //0:未更改，1:新建，2:修改
};

#endif // STORAGEUNIT_H
