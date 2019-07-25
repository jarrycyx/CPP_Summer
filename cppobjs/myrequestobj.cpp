//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Data
/************************************************************************************************************************
类名：     MyArticleObj
功能：     本地存储请求内容，实现对请求的增添、状态更改等操作方法
备注：     type各状态意义如下
            1	报名为负责人
            2	报名为译者
            3   负责人对译者的反馈

日期：     20190720 初步实现该类的Regulator部分
                 20190722 实现Translator部分
************************************************************************************************************************/

#include "myrequestobj.h"

/*************************************************************************
名称：     setContent等
功能：     设置和获取各项参数
日期：     20190717
*************************************************************************/
MyRequestObj::MyRequestObj(const int &newRequestId, const int &newUserId,
                           const int &newArticleId, const int &newType)
    : StorageUnit(StorageUnit::Unchanged), requestId(newRequestId), articleId(newArticleId), userId(newUserId), type(newType)
    //初始更改状态为0：未更改
{
}


void MyRequestObj::setContent(QString str)
{
    if (str != content)
    {
        content = str;
        if (getModifyStatus() == StorageUnit::Unchanged)
            setModifyStatus(StorageUnit::Changed); //标记为已修改，若本身状态为“新增”，则无需修改为“已修改”状态
    }
}
