#include "myrequestobj.h"

MyRequestObj::MyRequestObj(const int &newRequestId, const int &newUserId,
                           const int &newArticleId, const int &newType):
    StorageUnit (0) ,requestId(newRequestId), articleId(newArticleId), userId(newUserId), type(newType){
}
