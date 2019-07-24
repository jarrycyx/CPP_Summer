/************************************************************************************************************************
类名：     ArticleList
功能：     用于文章列表显示，继承QAbstractListModel，作为界面显示的Model
备注：     该类用于多个界面，在外部调用构造函数时应指定Type，即是否允许进行更改操作
                 实际因尽量减少在此类中直接进行修改操作，应该通过返回的指针在MyArticleObj中操作
日期：     20190720 初步实现该类，与ListView联调
                 20190721 进一步完善，增加全部删除等方法
************************************************************************************************************************/

//TO-DO: 使用类模板进行优化
//TO-DO: 重载[]运算符


#include "articleslist.h"

/************************************************************************************************************************
函数名：     ArticlesList
功能：         构造函数
参数：         int type，用于标注列表类型
日期：         20190720 实现构造函数
                     20190722 增加type属性
************************************************************************************************************************/
ArticlesList::ArticlesList(int type) : QAbstractListModel(), typeOfThisList(type)
{
}


/************************************************************************************************************************
函数名：     rowCount
功能：         Qt要求重载函数，向界面返回总行数
参数：         QModelIndex
返回值：     总行数
日期：         20190720
************************************************************************************************************************/
int ArticlesList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return articles.size();
}
/************************************************************************************************************************
函数名：     data
功能：         Qt要求重写函数，用于返回所需数据
参数：         const QModelIndex &index, int role，后者用于标注所请求的数据变量名
返回值：     QVariant
日期：         20190720 实现title和content
                     20190722 实现statusCode，type
                     20190724 实现Translated各类信息
************************************************************************************************************************/
QVariant ArticlesList::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return QVariant();

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
    }
}

/************************************************************************************************************************
函数名：     roleNames
功能：         Qt要求重写函数，用于返回各变量名对应序号的表
返回值：     QHash
日期：         20190720 实现title和content
                     20190722 实现statusCode，type
                     20190724 实现Translated各类信息
************************************************************************************************************************/
QHash<int, QByteArray> ArticlesList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[1] = "titleOfArticle";
    roles[2] = "contentOfArticle";
    roles[3] = "statusCodeOfArticle";
    roles[4] = "typeOfList";
    roles[5] = "translatedTitle";
    roles[6] = "translatedContent";
    return roles;
}

/************************************************************************************************************************
函数名：     getArticle，editAnArticle
功能：         获取article 的指针，用于获取信息或修改信息
参数：         文章在列表中的index，以及各文章信息
返回值：     QHash
日期：         20190720 实现title和content
                     20190722 实现statusCode，type
                     20190724 实现Translated各类信息
************************************************************************************************************************/
MyArticleObj *ArticlesList::getArticle(int idx)
{
    return articles[idx];
}

//虽然不建议在此类中直接编辑，但仍保留该函数接口
void ArticlesList::editAnArticle(int index, QString title, QString content)
{
    articles[index]->setTitleOfArticle(title);
    articles[index]->setContentOfArticle(content);
    QModelIndex idx = createIndex(index, index);                                  //调用Qt接口生成所需Index
    emit dataChanged(idx, idx);                                                                 //发送视图刷新信号
}

//实际用于通知视图刷新
void ArticlesList::editAnArticle(int index)
{
    QModelIndex idx = createIndex(index, index);
    emit dataChanged(idx, idx);
}

void ArticlesList::addAnArticle(MyArticleObj *newArticle)
{
    beginInsertRows(QModelIndex(), 0, 0);                                                 //开始刷新
    articles.push_front(newArticle);
    endInsertRows();                                                                                       //结束刷新
}

void ArticlesList::deleteAnArticle(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    articles[index]->setModifyStatus(3);
    articles.remove(index);
    endRemoveRows();
}
