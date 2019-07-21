#ifndef REQUESTUSERLIST_H
#define REQUESTUSERLIST_H


#include <QObject>
#include <QAbstractListModel>
#include "../cppobjs/myuserobj.h"

class RequestUserList: public QAbstractListModel
{
public:
    explicit RequestUserList();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addARequestUser(MyUserObj* newRequestUser);

    inline MyUserObj* getRequestUser(int idx){return requestUsers[idx];}

    inline void removeAllRequestUsers(){requestUsers.clear();}

    Q_INVOKABLE void removeOne();

protected:
    QVector<MyUserObj*> requestUsers;
    QHash<int, QByteArray> roleNames() const override;
};
#endif // REQUESTUSERLIST_H
