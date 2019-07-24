//总体框架：             Storage - Data - Model - Interaction - View
//该类所处层级：     Model
/************************************************************************************************************************
类名：     ArticleList
功能：     用于文章列表显示，继承QAbstractListModel，作为界面显示的Model
                 本类是沟通Model与Interaction的桥梁
备注：     该类用于多个界面，在外部调用构造函数时应指定Type，即是否允许进行更改操作
                 实际因尽量减少在此类中直接进行修改操作，应该通过返回的指针在MyArticleObj中操作
                 该类的主要目的是通过Qt各对象和QML视图直接交互
日期：     20190720 初步实现该类，与ListView联调
                 20190721 进一步完善，增加全部删除等方法
************************************************************************************************************************/

//TO-DO: 使用类模板进行优化
//TO-DO: 重载[]运算符


#ifndef ARTICLESLIST_H
#define ARTICLESLIST_H

#include <QObject>
#include <QAbstractListModel>
#include "../cppobjs/myarticleobj.h"

class ArticlesList : public QAbstractListModel
{
public:
    //构造函数中必须指定Type，为1表示自己的文章，可以进行删除更改等操作
    explicit ArticlesList(int type);
    //Qt要求重载的方法，用于计算总的行数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //Qt要求重载的方法，用于查询某个数据
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //增加文章
    void addAnArticle(MyArticleObj *newArticle);
    //修改文章，传入新的文章和内容，同时通知界面进行更改
    void editAnArticle(int index, QString title, QString content);
    void editAnArticle(int index);
    //删除文章，同时通知界面进行更改
    void deleteAnArticle(int index);
    //删除所有项
    inline void removeAllArticles() { articles.clear(); }
    //获取文章的指针，用于实际的修改操作
    MyArticleObj *getArticle(int idx);

protected:
    //存储所有文章的数据结构，由于需要保证文章对象实体只有一份，使用指针
    QVector<MyArticleObj *> articles;
    //存储QML界面需要查询的变量名
    QHash<int, QByteArray> roleNames() const override;
    int typeOfThisList;
};

#endif // ARTICLESLIST_H
