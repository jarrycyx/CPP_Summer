#include "myuserobj.h"

MyUserObj::MyUserObj() : StorageUnit(StorageUnit::Unchanged)
{
}

MyUserObj::MyUserObj(const int &user_id, const QString &name, const QString &password, int role)
    : StorageUnit(StorageUnit::Unchanged), mUsername(name), mPassword(password),
      mUserId(user_id), mRole(role)
    //初始更改状态为0：未更改
{
}


/*************************************************************************
名称：     username等
功能：     设置和获取各项参数
日期：     20190712
*************************************************************************/
QString MyUserObj::username() const
{
    return mUsername;
}

void MyUserObj::setUsername(const QString &name)
{
    if (name != mUsername)
    {
        mUsername = name;
        if (getModifyStatus() == StorageUnit::Unchanged)
            setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
    }
}

QString MyUserObj::password() const
{
    return mPassword;
}

void MyUserObj::setPassword(const QString &pswd)
{
    if (pswd != mPassword)
    {
        mPassword = pswd;
        if (getModifyStatus() == StorageUnit::Unchanged)
            setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
    }
}

int MyUserObj::userId() const
{
    return mUserId;
}

int MyUserObj::role() const
{
    return mRole;
}



QString MyUserObj::qualification() const
{
    return mQualification;
}
void MyUserObj::setQualification(QString newQuali)
{
    mQualification=newQuali;
}
int MyUserObj::money() const
{
    return mMoney;
}
void MyUserObj::setMoney(int newMoney)
{
    mMoney=newMoney;
}
int MyUserObj::credit() const
{
    return mCredit;
}
void MyUserObj::setCredit(int newCredit)
{
    mCredit=newCredit;
}
