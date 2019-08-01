/*************************************************************************
类名：     ArticleList
功能：     用于文章列表显示，继承QAbstractListModel，作为界面显示的Model
备注：     该类用于多个界面，在外部调用构造函数时应指定Type，即是否允许进行更改操作
                 实际因尽量减少在此类中直接进行修改操作，应该通过返回的指针在MyArticleObj中操作
日期：     20190720 初步实现该类，与ListView联调
                 20190721 进一步完善，增加全部删除等方法
*************************************************************************/


#include "articleslist.h"

/*************************************************************************
名称：     ArticlesList
功能：     构造函数
参数：     int type，用于标注列表类型
日期：     20190720 实现构造函数
          20190722 增加type属性
*************************************************************************/
ArticlesList::ArticlesList(int type) : QAbstractListModel(), typeOfThisList(type)
{
}

/*************************************************************************
名称：     rowCount
功能：     Qt要求重载函数，向界面返回总行数
参数：     QModelIndex
返回：     总行数
日期：     20190720
*************************************************************************/
int ArticlesList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return articles.size();
}
/*************************************************************************
名称：     data
功能：     Qt要求重写函数，用于返回所需数据
参数：     const QModelIndex &index, int role，后者用于标注所请求的数据变量名
返回：     QVariant
日期：     20190720 实现title和content
          20190722 实现statusCode，type
          20190724 实现Translated各类信息
*************************************************************************/
QVariant ArticlesList::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return QVariant();
    /*
    roles[1] = "titleOfArticle";
    roles[2] = "contentOfArticle";
    roles[3] = "statusCodeOfArticle";
    roles[4] = "typeOfList";
    roles[5] = "translatedTitle";
    roles[6] = "translatedContent";
    roles[7] = "articleId";
    */

    switch (role)
    {
    case 1:
        return articles.at(index.row())->titleOfArticle();
    case 2:
        return articles.at(index.row())->contentOfArticle();
    case 3:
        return articles.at(index.row())->statusCodeOfArticle();
    case 4:
        return typeOfThisList;
    case 5:
        return articles.at(index.row())->translatedTitle();
    case 6:
        return articles.at(index.row())->translatedContent();
    case 7:
        return articles.at(index.row())->articleIdOfArticle();
    }
}

/*************************************************************************
名称：     roleNames
功能：     Qt要求重写函数，用于返回各变量名对应序号的表
返回：     QHash
日期：     20190720 实现title和content
          20190722 实现statusCode，type
          20190724 实现Translated各类信息
*************************************************************************/
QHash<int, QByteArray> ArticlesList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[1] = "titleOfArticle";
    roles[2] = "contentOfArticle";
    roles[3] = "statusCodeOfArticle";
    roles[4] = "typeOfList";
    roles[5] = "translatedTitle";
    roles[6] = "translatedContent";
    roles[7] = "articleId";
    return roles;
}

/*************************************************************************
名称：     getArticle，editAnArticle
功能：     获取article 的指针，用于获取信息或修改信息
参数：     文章在列表中的index，以及各文章信息
返回：     QHash
日期：     20190720 实现title和content
          20190722 实现statusCode，type
          20190724 实现Translated各类信息
*************************************************************************/
MyArticleObj *ArticlesList::getArticle(int idx)
{
    return articles[idx];
}

/*************************************************************************
名称：     editAnArticle
功能：     编辑一项
参数：     index，以及文章内容
返回：     无
日期：     20190712
*************************************************************************/
void ArticlesList::editAnArticle(int index, QString title, QString content)
{ //虽然不建议在此类中直接编辑，但仍保留该函数接口
    articles[index]->setTitleOfArticle(title);
    articles[index]->setContentOfArticle(content);
    QModelIndex idx = createIndex(index, index); //调用Qt接口生成所需Index
    emit dataChanged(idx, idx);                  //发送视图刷新信号
}

/*************************************************************************
名称：     editAnArticle
功能：     刷新一项
参数：     列表中位置index
返回：     无
日期：     20190715
*************************************************************************/
void ArticlesList::editAnArticle(int index)
{ //实际用于通知视图刷新一项
    QModelIndex idx = createIndex(index, index);
    emit dataChanged(idx, idx); //发送视图刷新信号
}

/*************************************************************************
名称：     addAnArticle
功能：     新增一项
参数：     新增的文章对象指针
返回：     无
日期：     20190712
*************************************************************************/
void ArticlesList::addAnArticle(MyArticleObj *newArticle)
{
    beginInsertRows(QModelIndex(), 0, 0); //开始刷新
    articles.push_front(newArticle);
    endInsertRows(); //结束刷新
}

/*************************************************************************
名称：     deleteAnArticle
功能：     删除一项
参数：     文章在列表中位置index
返回：     无
日期：     20190712
*************************************************************************/
void ArticlesList::deleteAnArticle(int index)
{
    beginRemoveRows(QModelIndex(), index, index); //通知视图刷新信号，由基类完成
    articles[index]->setModifyStatus(StorageUnit::Deleted);
    articles.remove(index);
    endRemoveRows(); //结束刷新
}

/*************************************************************************
名称：     refreshAll
功能：     刷新所有项
参数：     无
返回：     无
日期：     20190713
*************************************************************************/
void ArticlesList::refreshAll()
{
    emit dataChanged(createIndex(0, 0), createIndex(articles.length() - 1, 0));
}
