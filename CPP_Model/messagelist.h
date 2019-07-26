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
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addAMessage(MyRequestObj *newMessage);

    inline MyRequestObj *getMessage(int idx) { return messages[idx]; }

    inline void removeAllRequestUsers() { messages.clear(); }

    Q_INVOKABLE void removeOne();

protected:
    QVector<MyRequestObj*> messages;
    QHash<int, QByteArray> roleNames() const override;
};
#endif // MESSAGELIST_H
