#ifndef MYREQUESTOBJ_H
#define MYREQUESTOBJ_H

#include "storageunit.h"
#include <QString>

class MyRequestObj : public StorageUnit
{
public:
    MyRequestObj(const int &newRequestId, const int &newUserId,
                 const int &newArticleId, const int &newType);
    inline int getRequestId() { return requestId; }
    inline int getUserId() { return userId; }
    inline int getArticleId() { return articleId; }
    inline int getType() { return type; }

private:
    int requestId;
    int articleId;
    int userId;
    int type;
    QString content;
};

#endif // MYREQUESTOBJ_H
