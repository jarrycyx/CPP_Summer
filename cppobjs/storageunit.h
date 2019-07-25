//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Storage-Data
/************************************************************************************************************************
类名：     StorageUnit
功能：     本地存储内容的基类，实现对存储状态的标记，用于程序结束时对服务器信息的更新
备注：     modifyStatus各状态意义如下
                    0  未修改
                    1	新增
                    2  已修改
                    3  已删除
日期：     20190717 从文章、用户的类中抽离成单独的基类
************************************************************************************************************************/

//TO-DO: 增添Comment部分

#ifndef STORAGEUNIT_H
#define STORAGEUNIT_H

class StorageUnit
{
public:
    enum ModifyStatus{
        Unchanged = 0,
        New,
        Changed,
        Deleted
    };
    StorageUnit(ModifyStatus newStatus);
    int getModifyStatus();
    void setModifyStatus(ModifyStatus m);

private:
    ModifyStatus modifyStatus; //0:未更改，1:新建，2:修改，3：删除
};

#endif // STORAGEUNIT_H
