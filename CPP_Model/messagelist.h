//总体框架：     Storage - Data - Model - Interaction - View
//所处层级：     Data-Model
/************************************************************************************************************************
类名：     MessageList
功能：     用于文章列表显示，继承QAbstractListModel，作为界面显示的Model
                 本类是沟通Model与Interaction的桥梁
备注：     该类用于多个界面，在外部调用构造函数时应指定Type，即是否允许进行更改操作
                 实际因尽量减少在此类中直接进行修改操作，应该通过返回的指针在MyArticleObj中操作
                 该类的主要目的是通过Qt各对象和QML视图直接交互
日期：     20190726 初步实现该类
          20190721 进一步完善，调整界面
************************************************************************************************************************/


#ifndef MESSAGELIST_H
#define MESSAGELIST_H


#include <QObject>
#include <QAbstractListModel>
#include "../CPP_Data/myuserobj.h"
#include "../CPP_Data/myrequestobj.h"

class MessageList : public QAbstractListModel
{
public:
    explicit MessageList();
    //Qt要求重载的方法，用于计算总的行数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //Qt要求重载的方法，用于查询某个数据
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //增加一条消息
    void addAMessage(MyRequestObj *newMessage);
    inline MyRequestObj *getMessage(int idx) { return messages[idx]; }
    inline void removeAllRequestUsers() { messages.clear(); }

protected:
    QVector<MyRequestObj*> messages;
    QHash<int, QByteArray> roleNames() const override;
};
#endif // MESSAGELIST_H
